# MiniReddis

MiniReddis is a small Redis-inspired key-value store written in C. It runs as an interactive command-line program and keeps data in memory while supporting strings, lists, sets, hashes, expiration, persistence, and transactions.

This project is intended as a compact C data-structures project and learning exercise. It is not intended to be a drop-in Redis replacement or a network server.

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
