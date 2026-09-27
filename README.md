# MiniReddis

MiniReddis is a small Redis-inspired key-value store written in C. It runs as an interactive command-line program and keeps data in memory while supporting strings, lists, sets, hashes, expiration, persistence, and transactions.

This project is intended as a compact C data-structures project and learning exercise. It is not intended to be a drop-in Redis replacement or a network server.

## Project Proposal

### Project Description

MiniReddis is a command-line, Redis-inspired key-value store implemented in C. The project provides a compact example of how common data structures can be combined into a usable storage engine. Users can create keys, store different kinds of values, query and modify those values, expire keys, and group changes into transactions.

The application uses an in-memory hash table for fast key lookup. Mutating commands are also written to a local command log so the database can be reconstructed when the program starts again.

### Project Goals

The project goals are to:

1. Build a functional key-value store using standard C and dynamic memory.
2. Demonstrate practical implementations of strings, linked lists, sets, and hashes.
3. Provide a simple interactive interface with predictable commands and output.
4. Support basic database features such as expiration, persistence, and transactions.

### Future Goals
1. Add an LRU cache to make lookups even faster.
2. Make the program handle multiple concurrent users.
3. Implement a custom network protocol to allow clients to communicate with the server over TCP.

### Specifications

#### Functional Specifications

- The program accepts commands from standard input until the user enters `exit`.
- Each key stores exactly one value type: string, list, set, or hash.
- String operations support setting, reading, updating, and deleting values.
- List operations support insertion and removal from both ends and displaying contents.
- Set operations reject duplicate values and support removal and membership queries.
- Hash operations support field/value insertion, lookup, deletion, and display.
- Keys may receive an expiration time in seconds and can be queried with `TTL`.
- A transaction can be started with `BEGIN`, finalized with `COMMIT`, or undone with `ROLLBACK`.
- Successful mutating commands are appended to `Database.txt` for replay on startup.
- Invalid commands, missing keys, and operations using the wrong data type produce an error message.

#### Technical Specifications

- Language: C11
- Compiler settings: `-Wall -Wextra -std=c11`
- Build tool: GNU Make
- Storage index: fixed-size hash table with 10 buckets
- Collision handling: linked chaining within each bucket
- Memory management: dynamic allocation with explicit cleanup for every supported value type
- Persistence format: one space-separated command per line in `Database.txt`
- Interface: interactive command-line input through standard input and output
- Source organization: implementation files in `src/` and public headers in `include/`

#### Constraints

- Commands and values are space-separated, so values cannot contain spaces.
- Keys are limited by the fixed key buffer defined in `types.h`.
- String, list, and set values are limited by the fixed value buffers defined in `types.h`.
- The application is local and single-process; it does not provide networking or concurrent clients.
- Expiration metadata is held in memory and is not preserved in the persistence log.

### System Design

The system is organized into small modules with clear responsibilities:

```text
User input
	|
	v
src/main.c
	|
	+--> store.c       String keys, lookup, deletion, expiration, cleanup
	+--> list.c        List operations
	+--> set.c         Set operations
	+--> hash.c        Hash-field operations
	+--> transaction.c Snapshot, commit, and rollback
	`--> persist.c     Command logging and database replay
			 |
			 v
		Database.txt
```

Every key is represented by an `Entry` structure. The entry records the key name, value type, value pointer, expiration time, and link to the next entry in the same hash bucket. Lists, sets, and hashes use their own linked-node structures, while strings use dynamically allocated character storage.

Transactions create a deep copy of the current table. `COMMIT` frees the snapshot and keeps the current state; `ROLLBACK` frees the current state and restores the snapshot. This design keeps transaction behavior independent from the original entries.

## Features

- String values with `SET`, `GET`, and `DEL`
- Linked-list values with left and right push/pop operations
- Set values with duplicate detection and membership checks
- Hash values containing field/value pairs
- Key expiration and time-to-live queries
- In-memory transactions with commit and rollback
- Command-log persistence through `Database.txt`
- Separate source and header directories

## Requirements

- GCC with C11 support
- GNU Make
- A terminal or command prompt

On Windows, MinGW or MinGW-w64 provides GCC and GNU Make. On Linux and macOS, install GCC or Clang together with GNU Make.

## Build

From the project root, run:

```sh
make
```

The build creates:

```text
build/MiniReddis
```

On Windows, the executable is normally `build/MiniReddis.exe`.

Remove all generated objects and executables with:

```sh
make clean
```

The Makefile uses `-Wall -Wextra -std=c11` and places build artifacts in the ignored `build/` directory.

## Run

Linux and macOS:

```sh
./build/MiniReddis
```

Windows PowerShell:

```powershell
.\build\MiniReddis.exe
```

Enter `exit` to close the program. A short session looks like this:

```text
> SET greeting hello
> GET greeting
key: greeting,value: hello
> LPUSH queue first
> RPUSH queue second
> LRANGE queue
first->second->null
> exit
```

## Command Reference

Commands use space-separated arguments. Values cannot contain spaces.

| Data type | Commands | Example |
| --- | --- | --- |
| Strings | `SET key value`, `GET key`, `DEL key` | `SET language C` |
| Lists | `LPUSH key value`, `RPUSH key value`, `LPOP key`, `RPOP key`, `LRANGE key` | `LPUSH tasks compile` |
| Sets | `SADD key value`, `SREM key value`, `SISMEMBER key value`, `SMEMBERS key` | `SADD languages C` |
| Hashes | `HSET key field value`, `HGET key field`, `HDEL key field`, `HGETALL key` | `HSET user name Alice` |
| Expiration | `EXPIRE key seconds`, `TTL key` | `EXPIRE session 60` |
| Transactions | `BEGIN`, `COMMIT`, `ROLLBACK` | `BEGIN` |

Keys can hold one data type at a time. Using a command for a different type returns `Wrong data type`.

## Persistence

Successful mutating commands are appended to `Database.txt`. The file is replayed when MiniReddis starts, so data from a previous run can be restored.

`Database.txt` is local runtime data and is intentionally ignored by Git. Delete it before a run when starting with a completely empty database. Expiration timestamps are held in memory and are not persisted in the command log.

## Project Layout

```text
.
|-- src/          C implementation files
|-- include/      Public header files
|-- Makefile      Build and cleanup rules
|-- LICENSE       MIT license
|-- README.md     Project documentation
`-- Database.txt  Local runtime command log (ignored)
```

## License

This project is licensed under the MIT License. See [LICENSE](LICENSE) for details.
