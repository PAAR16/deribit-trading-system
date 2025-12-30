# Architecture Documentation

This document provides detailed technical architecture information for the Deribit Trading System. For a project overview, see [README.md](../README.md).

## System Architecture Overview

```mermaid
graph TB
    A[CLI Layer<br/>TradingCLI] --> B[Engine Layer<br/>TradingEngine]
    B --> C[API Layer<br/>IDeribitApi]
    C --> D[WebSocket Client<br/>DeribitWsClient]
    B --> E[Core Layer<br/>Order, Position,<br/>AccountSummary]
    D --> F[Deribit Exchange<br/>WebSocket API]
    B --> G[Infra Layer<br/>Logger]

    A --> H[Command Pattern<br/>BuyCommand,<br/>SellCommand, etc.]

    I[Strategy Pattern<br/>TradingStrategy] -.-> B

    classDef layer fill:#e1f5fe,stroke:#01579b,stroke-width:2px
    classDef external fill:#fff3e0,stroke:#e65100,stroke-width:2px
    classDef pattern fill:#f3e5f5,stroke:#4a148c,stroke-width:2px

    class A,B,C,E,G layer
    class F external
    class H,I pattern
```

## Data Flow Diagram

```mermaid
sequenceDiagram
    participant U as User
    participant C as TradingCLI
    participant E as TradingEngine
    participant A as DeribitWsClient
    participant D as Deribit API

    U->>C: Enter command (e.g., "buy BTC 1.0 50000")
    C->>E: executeCommand()
    E->>A: placeOrder(order)
    A->>D: WebSocket request
    D-->>A: Response
    A-->>E: Callback with result
    E-->>C: Display result
    C-->>U: Show response
```

## Component Responsibilities

| Component | Responsibility | Key Classes | Technologies |
|-----------|----------------|-------------|--------------|
| **CLI Layer** | User interaction, command parsing | `TradingCLI`, `Command`, `BuyCommand` | Command Pattern, Input parsing |
| **Engine Layer** | Business logic, orchestration | `TradingEngine`, `TradingStrategy` | Strategy Pattern, Business rules |
| **API Layer** | External communication | `IDeribitApi`, `DeribitWsClient` | WebSocket++, Boost.Asio, JSON |
| **Core Layer** | Domain models | `Order`, `Position`, `AccountSummary` | Domain-driven design |
| **Infra Layer** | Cross-cutting concerns | `Logger` | Logging, utilities |

## Design Patterns Implemented

### Command Pattern
- **Purpose**: Encapsulate trading commands as objects
- **Implementation**: `Command` base class with concrete implementations (`BuyCommand`, `SellCommand`, etc.)
- **Benefits**: Extensible CLI, undo/redo capability, command history

### Observer Pattern
- **Purpose**: Handle real-time market data subscriptions
- **Implementation**: Callback system for WebSocket responses and subscriptions
- **Benefits**: Decoupled event producers and consumers, real-time updates

### Strategy Pattern
- **Purpose**: Pluggable trading strategies
- **Implementation**: `TradingStrategy` base class for different algorithms
- **Benefits**: Easy to add new trading strategies without modifying core logic

### Dependency Injection
- **Purpose**: Loose coupling between components
- **Implementation**: `TradingEngine` receives `IDeribitApi` interface
- **Benefits**: Testable code, easy to swap implementations

### Interface Segregation
- **Purpose**: Client-specific interfaces
- **Implementation**: Separate `IDeribitApi` with focused methods
- **Benefits**: Reduced coupling, easier maintenance

## Technology Stack

- **Language**: C++17
- **Build System**: CMake
- **WebSocket**: WebSocket++ with Boost.Asio
- **JSON**: nlohmann/json
- **Cryptography**: OpenSSL
- **Threading**: C++ standard library threads
- **Logging**: Custom Logger class

## Performance Characteristics

- **Latency**: Sub-50ms for WebSocket communication
- **Throughput**: Handles real-time market data streams
- **Memory**: Efficient object management with smart pointers
- **Concurrency**: Thread-safe operations with mutex protection

## Security Considerations

- **Authentication**: Secure API key management
- **TLS**: Encrypted WebSocket connections
- **Input Validation**: Command parameter validation
- **Error Handling**: Comprehensive exception handling

## Future Enhancements

- **GUI Interface**: Qt-based desktop application
- **Database Integration**: Order history persistence
- **Advanced Strategies**: Machine learning-based trading algorithms
- **Multi-Exchange Support**: Abstract API for multiple exchanges
- **Testing Framework**: Unit and integration tests