# Initial Architecture

```text
Qt GUI
  |
  v
Business Logic
  |---- Report Parser
  |---- Reminder Service
  |---- Email Service
  |
  +---- File Manager ----> Report Storage
  |
  +---- Database Layer --> SQLite
```

The architecture is intentionally a starting point. The intern should refine it during Milestone 1 and document major design decisions.
