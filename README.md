# Sockets and DNS

A low-level networking project that replaces provided socket abstractions with custom implementations for TCP connection handling. The project implements core socket and DNS functionality while preserving existing client and server behavior.

---

## Overview

This implementation replaces internal networking helpers with bespoke versions that:
- Resolve hostnames using DNS
- Establish TCP connections between arbitrary hosts
- Accept incoming TCP connections
- Cleanly terminate connections

All existing clients (HTTP client/server and SMTP agent) continue to function without modification.

---

## Implemented Functions

Implemented in `src/sans_socket.c`:

- `sans_connect` – Resolve hostnames and establish TCP connections
- `sans_accept` – Bind, listen, and accept incoming TCP connections
- `sans_disconnect` – Terminate active connections

---

## Behavior

### sans_connect
- Verifies protocol is TCP
- Resolves hostnames using `getaddrinfo`
- Iterates through all returned addresses until a connection succeeds
- Creates and connects sockets using resolved address info

### sans_accept
- Verifies protocol is TCP
- Resolves interface addresses using `getaddrinfo`
- Creates, binds, and listens on sockets
- Iterates through available addresses on failure
- Blocks until an incoming connection is accepted

### sans_disconnect
- Terminates the connection using `close`

---

## Build

```bash
scons build
```

This produces:
- `sans` – main executable
- `sans-tests` – automated test suite

---

## Run Example

```bash
./sans http client www.example.com 80
```

**Input**
```
GET index.html
```

---

## Debugging

```bash
gdb --args ./sans http client www.example.com 80
```

The same approach can be used with test binaries.

---

## Notes

- All socket behavior matches the original implementation
- DNS resolution uses standard system facilities
- Designed for correctness, robustness, and compatibility with existing code

---

## Tech

- Language: C
- POSIX sockets
- DNS resolution via `getaddrinfo`
