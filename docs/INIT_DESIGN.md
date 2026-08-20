# split — initial design

What this program is, what it does, and how it's built. Syntax is deliberately
loose here — the concepts are settled, the exact spelling is not.

---

## 1. What it is

split is a small language for slicing text and data files. Point it at a CSV,
a log, or any structured text; it splits each row into fields, filters and
transforms them, and prints results.

It exists because AWK has the right shape but is 40 years old:

| AWK's problem | split's answer |
|---|---|
| Can't parse real CSV — breaks on `"Smith, John"` | Quote-aware splitting built in |
| Columns are numbers (`$3`) | Columns can have names (`price`) |
| Typos fail silently as empty strings | Unknown names are errors |
| No JSON, no structured output | Planned |
| Cryptic and unreadable | Readable keywords |

It's also meant to be **embeddable** — usable as a C++ library inside other
programs, not just a command-line tool.

---

## 2. The core model

**Execution is per row.** The script body runs once for every row of input,
top to bottom, then again for the next row.

A row is split into **fields**. Fields always exist and are reachable by
position. Field *names* are optional — they come from a header row, or from
names declared in the script. A file with no header is still fully usable.

Everything else follows from that: a filter decides which rows run the body, an
aggregate accumulates across rows, and output happens per row or once at the end.

---

## 3. What it needs to do

Five jobs, roughly in the order data flows through them.

### 3.1 Get data in

- Built-in formats for the common cases: CSV, TSV, whitespace-separated,
  raw lines
- Custom splitting rules for everything else (see section 4)
- Read from a named file, or from stdin so it works in a pipeline

### 3.2 Select rows

- A filter expression that decides whether the body runs for this row
- Comparisons, boolean combinations, text matching (substring, prefix/suffix,
  regex)

### 3.3 Select and build columns

- Reference fields by position or name
- Arithmetic, and text joining
- Rename, reorder, drop, and add computed columns

### 3.4 Summarize

The part that makes it more than a filter:

- Running totals: sum, count, average, min, max
- **Grouping** — totals *per category*, not just one grand total. This is the
  single most valuable feature on the list; it's what sends people to Excel.
- Sorting and deduplication

### 3.5 Get data out

- Print rows or arbitrary expressions
- Structured output (CSV, JSON) with a controllable separator
- Output should pipe cleanly into other commands

### 3.6 The unglamorous half

None of the above matters without:

- Variables, conditionals, and user-defined functions
- Comments
- A defined rule for when a value is treated as text vs a number
- Error messages with line numbers that say what was wrong
- A one-liner mode, so quick jobs don't need a script file

---

## 4. Custom parsing rules

This is the differentiating feature. The format spec is a separate sublanguage
from the query code — a declarative block describing how bytes become rows,
followed by ordinary code operating on those rows.

Every line-oriented format decomposes into three questions:

1. **How is a record found?** Usually a newline. Sometimes a blank line
   (paragraph-separated logs), sometimes a repeating marker.
2. **How is a record split into fields?**
3. **What are the fields called?**

### Splitting primitives

- Split on a literal delimiter
- Split on a regex
- Split with quote awareness (proper CSV)
- Split at fixed character positions (fixed-width columns)
- **Split a limited number of times**, remainder into the last field
- Split from the right instead of the left

That fifth one is the primitive the whole design leans on. It's what makes
"only the *second* space is a delimiter" expressible:

```
"Grant Smith Jr 42"  split on space, max 2  ->  ["Grant", "Smith Jr 42"]
```

Timestamps are the classic case: split a log line 3 times and you get date,
time, level, and "everything else" as the message — intact, spaces and all.

### Naming

Names come from a header row, or are declared in the script. A log file has no
header, but *you* know what its fields mean, so you name them once at the top
and write readable conditions for the rest of the script.

---

## 5. Streaming vs loading

Two input modes, chosen explicitly by the script author:

- **Streaming** — one row at a time, nothing held in memory. Handles files
  larger than RAM. Cannot sort, index rows, or know the row count until the end.
- **Loading** — whole file in memory. Sorting and random access available.

These are genuinely different capability sets, and that's a feature: asking for
a sort in streaming mode should be **rejected at parse time** with an error
that names the fix, rather than failing mysteriously at runtime.

Auto-detecting which mode to use is deliberately rejected. Silently deciding
whether a 10GB file goes into RAM is exactly what makes a tool feel
unpredictable.

**Build streaming first.** It's the harder constraint; building under it
prevents an evaluator that assumes random access everywhere. Loading is then a
small addition — the same row-source interface, backed by a vector instead of
a file handle.

---

## 6. Keywords

A working list, not final. The reserved list should stay **short**: every
reserved word is a column name users can't have, and real data has columns
named `count`, `max`, and `end`.

| Group | Words |
|---|---|
| Input | read, load, stream, from, as, format, fields, names |
| Splitting | split, by, max, at, quoted, right, regex |
| Filtering | where, and, or, not, contains, matches, in, starts, ends |
| Transform | let, set, keep, drop, rename |
| Structure | begin, end, if, else, func, return |
| Output | print, emit, write, to |

**Aggregates stay out of this list.** sum, count, avg, min, max, sort, unique,
group, head, tail are better as ordinary function calls — it keeps common
column names usable and makes parsing simpler.

Design principle throughout: **a small core plus escape hatches.** Roughly
fifteen built-in operations, one way to define your own functions, one way to
shell out. Comprehensiveness comes from combinations, not from a long feature
list. The test for any proposed feature: *could a user build this themselves
from what already exists?* If yes, leave it out.

---

## 7. How it's built

Four stages, each feeding the next:

```
script text  ->  LEXER    ->  tokens
tokens       ->  PARSER   ->  AST
data file    ->  READER   ->  rows
AST + rows   ->  INTERP   ->  output
```

**Lexer** — turns script characters into tokens: identifiers, numbers, strings,
field references, keywords, operators, newlines. Handles comments and
whitespace. *Working.*

**Parser** — recursive descent over the token list, producing an abstract
syntax tree. Handles operator precedence. *Not started.*

**Reader** — the format layer from section 4. Turns a data file into a stream
of rows. Independent of the parser; the two meet only in the interpreter.

**Interpreter** — walks the AST once per row, evaluating expressions against
the current row's fields.

### Embedding

The interpreter is a library first, the CLI a thin wrapper around it. Three
constraints that are painful to retrofit and free to adopt now:

1. **A `Value` type** — text or number, with explicit accessors. This is the
   boundary between split and any host program.
2. **Never call `exit()` on an error.** A CLI can die; a library embedded in
   someone else's program cannot. Errors return a status or throw.
3. **No printing inside the interpreter.** Output goes through something the
   caller controls.

---

## 8. Build order

Each stage should run end to end before the next begins.

1. Lexer producing a printable token stream — *done*
2. Parser handling expressions only, with a printable AST
3. Parser handling filter-and-block statements
4. Reader: a data file into rows, positional fields only
5. Interpreter: filter rows and print fields
6. Variables, accumulation, and a run-at-the-end block
7. Named fields from header rows
8. Custom format blocks
9. Aggregation and grouping
10. Structured output

The whole thing is useful at step 6 and genuinely good at step 9.

---

## 9. Still to work through

- String escape handling — decoded by the lexer or left raw?
- The rule for text vs numeric comparison. Whatever it is, it must be written
  down and applied consistently; this is the biggest source of surprise in
  AWK-like languages.
- Whether keywords and operators get individual token types, or stay as two
  buckets the parser compares by string. Decide before writing the parser.
- Operator precedence table
- Statement terminator: newline, semicolon, or both
- XML and JSON are tree problems, not delimiter problems — they need a
  selector concept to flatten a tree into rows. Deferred. Markdown is out of
  scope; there is no single spec for it.