# Заметки на будущее

## Multiplayer / movement

- `p.NetEnableMoveCombining=0` можно попробовать позже как опциональный workaround для listen-server client movement jitter.
- Что делает: отключает объединение client movement updates, из-за чего клиент чаще отправляет movement-пакеты на host/server.
- Потенциальный плюс: движение remote client на listen-server может выглядеть отзывчивее и чаще обновляться.
- Потенциальный минус: больше client -> server movement-трафика и выше packet rate. Для небольшого co-op это может быть приемлемо, но лучше проверять на 2-4 игроках и с имитацией плохой сети.

