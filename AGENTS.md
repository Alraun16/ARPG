# AGENTS.md — ARPG / Unreal Engine 5

## Core priorities

1. Minimize tool calls, file reads, and Codex limit usage.
2. Make the smallest localized change.
3. Do not run builds, tests, Unreal Editor, packaging, cooking, Git commands, or long commands unless I explicitly ask.
4. Do not re-read unchanged files just to verify yourself.
5. Explain changes clearly in Russian, but do not spend extra tool calls to make explanations longer.
6. Preserve Unreal C++ reflection rules, Blueprint compatibility, and asset safety.
7. Assume listen-server co-op multiplayer unless I say otherwise.

## Tool-use limits

- Read only files directly needed for the task.
- Read each relevant file once when possible.
- Do not scan the whole repository unless explicitly requested.
- Do not repeatedly inspect all `.h` / `.cpp` files after small edits.
- If a patch fails, re-read only the smallest relevant fragment and retry once.
- If a command fails, do not retry with guessed flags.
- Do not run `git diff`, `git status`, `Format-Hex`, encoding checks, ownership fixes, or broad diagnostics unless I explicitly ask.
- If verification is not cheap, skip it and tell me what to check manually.

## Unreal rules

- Keep `*.generated.h` as the last include.
- Do not remove or casually change `UCLASS`, `USTRUCT`, `UENUM`, `UFUNCTION`, `UPROPERTY`, `GENERATED_BODY`, or `UMETA`.
- Do not rename reflected classes/properties/functions/enums casually. Mention `CoreRedirects` if a rename is required.
- Do not edit `.uasset`, `.umap`, `Binaries`, `Intermediate`, `Saved`, `DerivedDataCache`, generated project files, or build artifacts.
- Do not add modules, plugins, dependencies, GAS, GameplayTags, UMG, OnlineSubsystem, rollback netcode, peer-to-peer sync, or dedicated-server-first architecture unless I explicitly ask.
- Use `CreateDefaultSubobject` for default components in constructors.
- Use `NewObject` for runtime `UObject` creation.
- Long-lived `UObject` references should be `UPROPERTY`.

## Multiplayer rules

The intended model is listen-server co-op.

- One player is host/server and local player.
- Other players connect as clients.
- Server/host owns authoritative gameplay state.
- Clients may use local cosmetic prediction for responsiveness.
- Server confirms actual gameplay results.
- Do not let clients authoritatively decide damage, death, loot, enemy AI, or persistent world state.
- For replicated state, prefer small incremental changes.
- Do not assume `SetIsReplicated(true)` replicates nested `UObject` state.

## Attribute system rules

The project uses a custom lightweight attribute system, not GAS.

- Preserve separation between gameplay state, component ownership, and ViewModel/UI-facing data.
- Attribute changes should broadcast events consistently.
- Clamp current values safely.
- Server should be authoritative for gameplay attribute changes.
- ViewModels/UI must not become the source of truth.

## Response format after edits

Answer in Russian:

1. Изменённые файлы.
2. Что изменено.
3. Объяснение для понимания.
4. Допущения.

For small changes, keep the explanation short.
For non-trivial UE/C++/networking changes, explain the involved concept clearly, using already-read context and the actual diff.