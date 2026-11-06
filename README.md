# CPP Module 09

STL containers in practice: a Bitcoin exchange-rate lookup, a Reverse Polish Notation calculator, and Ford–Johnson sorting with `std::vector` and `std::deque`.

> Part of the **42 school curriculum**. Built by **Arda Aycan**.

## Exercises

| Exercise | Binary | What it demonstrates |
| --- | --- | --- |
| `ex00` | `btc` | Stores Bitcoin exchange rates in `std::map`, validates input dates and values, and uses the closest earlier database date when an exact date is absent. |
| `ex01` | `RPN` | Evaluates Reverse Polish Notation with `std::stack` (backed by `std::list`), validating tokens, operands, division by zero and integer overflow. |
| `ex02` | `PmergeMe` | Implements Ford–Johnson merge-insertion sorting separately with `std::vector` and `std::deque`, using Jacobsthal insertion order and reporting each container's processing time. |

## Build and run

Each exercise has its own Makefile and binary:

```bash
make -C ex00
(cd ex00 && ./btc input.txt)

make -C ex01
./ex01/RPN "8 9 * 9 - 9 - 9 - 4 - 1 +"

make -C ex02
./ex02/PmergeMe 3 5 9 7 4
```

Run `make -C exNN fclean` to remove an exercise's binary and object files.

## Notes

- C++98, compiled with `-Wall -Wextra -Werror`.
- Containers cannot be reused between exercises: `std::map` is used in ex00, `std::stack` with its underlying `std::list` in ex01, and `std::vector` plus `std::deque` in ex02.
- ex00 opens `data.csv` relative to the current working directory, so run `btc` from inside `ex00`.
- ex02 reports timings in microseconds for each container's setup and sorting work.

## Author

**Arda Aycan** ([@EpicYamo](https://github.com/EpicYamo)).
