# MiniReddis

MiniReddis is a small Redis-inspired in-memory key-value store written in C. It supports strings, lists, sets, hashes, key expiration, persistence, and transactions through an interactive command-line interface.

## Prerequisites

- GCC with C11 support
- GNU Make

## Build

```sh
make
```

The executable is created at `build/MiniReddis`. Build objects and binaries are ignored by Git.

To remove build outputs:

```sh
make clean
```

## Run

```sh
./build/MiniReddis
```

Example session:

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

Supported commands include:

- Strings: `SET`, `GET`, `DEL`
- Lists: `LPUSH`, `RPUSH`, `LPOP`, `RPOP`, `LRANGE`
- Sets: `SADD`, `SREM`, `SISMEMBER`, `SMEMBERS`
- Hashes: `HSET`, `HGET`, `HDEL`, `HGETALL`
- Expiration: `EXPIRE`, `TTL`
- Transactions: `BEGIN`, `COMMIT`, `ROLLBACK`

Commands that mutate data are appended to `Database.txt` and replayed when the program starts. The database file is local runtime state and is intentionally excluded from version control.

## Project Layout

- `src/`: C source files
- `include/`: public header files
- `Makefile`: build and cleanup commands
- `LICENSE`: MIT license

## License

This project is licensed under the MIT License. See `LICENSE` for details.
