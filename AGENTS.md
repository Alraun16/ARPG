# AGENTS.md — ARPG / Unreal Engine 5

## Main goal

Minimize Codex usage while still producing correct, reviewable Unreal Engine C++ changes.

This is an Unreal Engine 5.7 ARPG project written in C++.

Main repository: GitLab.  
GitHub is used as a read-only mirror for code analysis through ChatGPT/Codex.  
Repository for analysis: `Alraun16/ARPG`.

Primary areas:

- `Source/ARPG/Public`
- `Source/ARPG/Private`
- `Config`
- `Content` only for asset references, paths, maps, Blueprint-facing API assumptions, and manual follow-up notes.

The project uses Unreal Engine C++ conventions, Unreal Header Tool, reflection macros, Blueprint integration, `EnhancedInput`, and may include multiplayer/replication code.

## Priority order

1. Preserve project-specific ARPG architecture.
2. Minimize Codex usage, repeated file reads, and expensive commands.
3. Preserve Unreal reflection, Blueprint compatibility, and asset safety.
4. Preserve the intended listen-server co-op networking model.
5. Make changes understandable and explain Unreal/C++ reasoning clearly.
6. Make the smallest localized change.
7. Ask before broad refactors, new dependencies, new plugins, or architecture changes.

## Default behavior

- Do not build the project unless I explicitly ask you to.
- Do not run Unreal Editor, full builds, full test suites, full lint, full typecheck, dependency installs, migrations, dev servers, Docker builds, cooking, packaging, shader compilation, or long-running commands unless I explicitly approve.
- Do not run `GenerateProjectFiles`, `BuildCookRun`, full project regeneration, editor startup, or packaging commands unless explicitly requested.
- Prefer static inspection, targeted file reads, and minimal diffs.
- Do not explore the whole repository unless necessary. Start from the files directly relevant to the task.
- Do not make speculative refactors or cleanup unrelated to the request.
- Do not add dependencies, plugins, modules, or third-party libraries unless I explicitly approve.
- Do not repeatedly retry failing commands. If a command fails once, stop and summarize the failure.
- Do not compensate for skipped builds/tests by making broad code changes. Make the smallest safe change and tell me what to verify locally.

## Context and file reading budget

Minimize repeated file reads and repository exploration.

Within a single task:

- Read each relevant file once when possible.
- Do not re-read unchanged files just to reassure yourself.
- Do not repeatedly inspect all `.h` / `.cpp` files after small edits.
- Do not scan the whole repository unless the task explicitly requires broad search.
- Prefer a small working set of files for each task.
- After editing, use the diff or the changed hunk as the primary source for the summary.
- Re-open a file only if:
  - the previous read was incomplete;
  - the file changed after the previous read;
  - an error points to a specific line;
  - a new requirement makes that file relevant;
  - there is a concrete uncertainty that cannot be resolved from the current context.

For small localized edits, avoid repository-wide searches. Use targeted search only when the symbol, file, or dependency is unknown.

Do not perform “confidence checks” by repeatedly reading the same files. If uncertain, state the uncertainty and tell me what to verify manually.

For tiny edits, keep the working set tiny. A small change should not trigger a full project inspection.

## Reasoning effort

- Default reasoning effort: Medium.
- Use Low for cosmetic, localized, or clearly simple changes.
- Ask before using High.
- Never use Max or XHigh unless I explicitly request it.
- Do not compensate for lower reasoning by running builds, tests, lint, typecheck, Unreal Editor, packaging, or long commands automatically. Ask me to run verification manually.

## Language and terminology

- Communicate with me in Russian by default.
- Write all user-facing explanations in Russian: summaries, change descriptions, verification notes, risks, assumptions, questions, and status messages.
- Keep technical tokens in their original form and do not translate them:
  - file names and paths, for example `Source/ARPG/Public/Characters/MainPlayerCharacter.h`, `ARPG.Build.cs`, `DefaultEngine.ini`;
  - class, method, property, variable, namespace, enum, interface, delegate, macro, and event names;
  - Unreal terms such as `UCLASS`, `UPROPERTY`, `UFUNCTION`, `GENERATED_BODY`, `AActor`, `UObject`, `ACharacter`, `APlayerController`, `AGameModeBase`, `OnRep`, `RPC`, `DOREPLIFETIME`;
  - commands, flags, environment variables, package names, API names, module names, plugin names, error codes, compiler diagnostics, and exception names;
  - exact text from logs, build output, compiler messages, crash reports, and stack traces.
- Use Cyrillic for normal readable words intended for the user.
- Do not write full English user-facing sections such as `Summary of changed files`, `What changed`, `Verification needed`, or `Risks or assumptions`. Use Russian equivalents.
- Code comments should follow the existing language style of the file. If there is no clear existing style, use Russian for explanatory comments intended for humans, while keeping identifiers and technical terms in their original form.
- User-facing UI text in the game should follow the existing project language. If surrounding UI is Russian, write new UI text in Russian. If surrounding UI is English, preserve English unless I ask to translate it.

## Explanation and learning mode

I want to understand the project, the code, and the changes, not just receive finished patches.

Assume I have general programming experience, but I am still weak in C++, Unreal Engine, gameplay architecture, networking, and multiplayer implementation.

When making or proposing non-trivial changes:

- Explain what problem the change solves.
- Explain why this approach fits Unreal Engine and this ARPG project.
- Explain important C++ or Unreal-specific concepts directly involved in the change.
- Explain what each newly added class, function, property, macro, delegate, RPC, or replicated field is responsible for.
- Explain why any newly added `UPROPERTY`, `UFUNCTION`, `UCLASS`, `GENERATED_BODY`, `Replicated`, `ReplicatedUsing`, `OnRep`, `Server`, `Client`, or `NetMulticast` usage is needed.
- Explain ownership and lifetime when adding `UObject`, `UActorComponent`, `AActor`, pointers, references, or `NewObject`.
- Explain server/client responsibility when adding multiplayer code.
- Explain which parts are cosmetic/local and which parts are authoritative/server-owned.
- Explain Blueprint impact if a change affects reflected API, metadata, categories, exposed properties, or asset references.
- Explain any manual Unreal Editor steps that may be needed after the code change.
- Explain risks and tradeoffs briefly, especially if there are multiple plausible Unreal approaches.
- Prefer clear, practical explanations over terse expert shorthand.
- Do not expand explanations by reading extra files unless those files are directly relevant to the change.
- Explain from the actual diff and already-read context whenever possible.

Do not assume I already understand advanced Unreal or C++ details.

For simple changes, explain briefly.

For non-trivial Unreal/C++ changes, explain:

- what changed;
- why it was needed;
- what Unreal/C++ concept is involved;
- what I should watch for later.

Do not over-simplify into toy code. Aim for indie-solo-dev practicality with a path toward solid mid-level architecture:

- avoid over-engineered AAA-style systems unless explicitly requested;
- avoid primitive "just make it work" hacks that will obviously break soon;
- prefer small, understandable, maintainable steps;
- mention when a solution is a temporary scaffold and what a stronger later version would look like;
- keep the first implementation narrow, but do not hide important architectural consequences.

When choosing between approaches, briefly state:

1. Chosen approach.
2. Why it is appropriate now.
3. What was not chosen.
4. What might be upgraded later.

For complex changes, include a small conceptual map before or after the patch, for example:

```text
Input -> Character/Component -> Server RPC -> Server state change -> Replication/OnRep -> ViewModel/UI update
```

Use explanation depth appropriate to the change:

- simple localized fix: short explanation;
- new gameplay system: detailed explanation;
- networking/replication change: detailed explanation with server/client ownership;
- Unreal reflection/Blueprint-facing change: detailed explanation with asset compatibility notes.

## Safety rules

- Never delete files without separate explicit confirmation in the same message.
- Never run destructive commands such as `rm`, `del`, `rmdir`, `git clean`, `git reset --hard`, removing directories, or mass file deletion unless I explicitly approve that exact action.
- If deletion seems necessary, explain why and ask me to do it manually.
- Do not modify generated files, IDE-generated files, intermediate files, cache folders, or build artifacts unless explicitly requested.
- Do not modify Unreal binary assets such as `.uasset`, `.umap`, `.ubulk`, `.uexp`, or `.locres` as text.
- Do not modify `Binaries`, `Intermediate`, `Saved`, `DerivedDataCache`, generated Visual Studio project files, generated solution files, or generated build metadata unless explicitly requested.
- Do not change public APIs, Blueprint-exposed APIs, serialized property names, data formats, save formats, config schemas, or persisted state schemas unless required by the task.
- Do not rename reflected symbols casually. If a reflected class, property, function, enum, or struct must be renamed, mention whether `CoreRedirects` may be required.

## Before editing

For non-trivial tasks:

1. Briefly state the minimal files you plan to inspect.
2. State the smallest change you expect to make.
3. Mention the main Unreal/C++ concept involved if it matters.
4. Mention whether verification requires a build, test, lint, typecheck, editor run, PIE run, multiplayer PIE run, or manual run.

Keep this brief.

Do not announce a broad inspection unless the task truly requires it.

## During edits

- Make the smallest safe change.
- Preserve existing style and patterns.
- Prefer localized fixes over broad rewrites.
- Avoid unrelated cleanup.
- Avoid speculative architecture changes.
- Stop and ask before making changes that affect many files.
- If requirements are unclear, make a narrow assumption and state it briefly instead of exploring widely.
- Do not rewrite architecture unless the task explicitly asks for architecture work.
- Do not introduce new systems when a localized change is enough.
- Do not repeatedly re-open files after every small edit. Use the diff and changed hunks unless a concrete issue requires another read.

## Preferred inspection workflow

For code analysis:

1. Start from the smallest relevant file set.
2. Inspect the `.h` / `.cpp` pair only when both are needed.
3. Check `ARPG.Build.cs` only if new modules, includes, or dependencies may be required.
4. Check `Config` only if the change affects maps, GameMode, input, collision, redirects, project settings, console variables, or packaged-build behavior.
5. Check `Content` only for asset paths and Blueprint-facing assumptions.
6. Avoid re-reading files already inspected in the same task unless they changed or a specific uncertainty appears.
7. Propose or make the smallest change.

For implementation:

1. Modify only the files required by the task.
2. Keep `*.generated.h` last.
3. Add includes only when actually needed.
4. Update `ARPG.Build.cs` only if required and explicitly justified.
5. Mention manual Blueprint or Unreal Editor steps instead of trying to inspect or fix assets automatically.
6. After editing, summarize from the actual diff and already-read context.

Do not treat this workflow as permission to inspect the whole project. It is a narrowing workflow, not a broad checklist.

## Unreal Header Tool rules

When editing Unreal C++ headers:

- Keep `*.generated.h` as the last include in the header.
- Do not place includes after `*.generated.h`.
- Do not remove Unreal reflection macros:
  - `UCLASS`
  - `USTRUCT`
  - `UENUM`
  - `UINTERFACE`
  - `GENERATED_BODY`
  - `UPROPERTY`
  - `UFUNCTION`
  - `UMETA`
- Do not change reflected class, property, function, delegate, enum, or struct names casually.
- Do not change `BlueprintReadOnly`, `BlueprintReadWrite`, `BlueprintAssignable`, `EditAnywhere`, `VisibleAnywhere`, `Category`, `meta`, or replication specifiers casually.
- If a reflected name must be renamed, consider whether `CoreRedirects` are required in `Config`.
- Avoid templates or complex C++ constructs inside reflected declarations unless Unreal Header Tool supports them.
- Prefer Unreal-supported types in reflected APIs:
  - `TObjectPtr`
  - raw `UObject*` with `UPROPERTY`
  - `TSubclassOf`
  - `TArray`
  - `TMap`
  - `TSet`
  - `FName`
  - `FText`
  - `FString`
  - `FGameplayTag` only if the project explicitly uses `GameplayTags`.

## UObject and GC rules

- Any `UObject` reference that must survive garbage collection should be stored in a `UPROPERTY`.
- Use `CreateDefaultSubobject` only in constructors for default components.
- Use `NewObject` for runtime `UObject` creation.
- Do not use `new` / `delete` for `UObject` instances.
- Do not manually delete `UObject` instances.
- Be careful with object outers when using `NewObject`.
- Do not store long-lived `UObject` references in non-`UPROPERTY` raw pointers unless there is a clear reason.
- Be careful when creating nested `UObject` state inside replicated components; replication does not automatically serialize arbitrary nested object state.

## Components

For Actor Components:

- Create default components in actor constructors with `CreateDefaultSubobject`.
- Runtime-created components or objects must have correct ownership.
- If a component is intended for replication, configure replication explicitly.
- Avoid expensive logic in `TickComponent` unless needed.
- Prefer configurable intervals for periodic logic.
- Do not assume `SetIsReplicated(true)` alone replicates all internal state.

## Multiplayer model

The intended multiplayer model is listen-server co-op.

One player acts as both host/server and local player. Other players connect as clients. The host/server owns authoritative world state and replicates relevant gameplay state to clients.

This project is not pure peer-to-peer and is not dedicated-server-first.

Prioritize practical solo-developer implementation over competitive PvP-grade networking.

Default networking assumptions:

- Host/server is authoritative for gameplay state.
- Clients send input requests to the server for authoritative gameplay changes.
- Clients may use local cosmetic prediction for responsiveness.
- Server confirms, rejects, or corrects actual gameplay results.
- Use Unreal's built-in `CharacterMovementComponent` prediction where possible.
- Do not introduce dedicated-server-first architecture unless explicitly requested.
- Do not introduce rollback netcode unless explicitly requested.
- Do not introduce pure peer-to-peer synchronization unless explicitly requested.
- Do not introduce host migration unless explicitly requested.
- Do not add session/lobby/OnlineSubsystem logic unless explicitly requested.
- Do not optimize for competitive PvP latency unless explicitly requested.
- Keep replication changes incremental and testable for solo development.

For dynamic PvE ARPG combat, separate responsiveness from authority:

Local cosmetic prediction may include:

- attack animation start;
- dodge animation start;
- sound;
- VFX;
- weapon trails;
- camera shake;
- local input feel;
- non-authoritative anticipation UI.

Server-confirmed gameplay results include:

- damage;
- death;
- stamina, health, and spirit final values;
- cooldown final state;
- hit confirmation;
- stagger;
- enemy AI decisions;
- enemy position/state;
- loot;
- world state changes.

Do not let clients authoritatively decide damage, death, loot, enemy AI, or persistent world state.

If a predicted local action is rejected by the server, prefer a small correction over broad rewrites or complex rollback unless explicitly requested.

## Replication rules

If code is multiplayer-related:

- Assume listen-server co-op unless the task explicitly says otherwise.
- Server/host should own authoritative gameplay state.
- Client input should call server RPCs when changing authoritative state.
- Replicated properties must use `GetLifetimeReplicatedProps`.
- Use `DOREPLIFETIME` or the correct Unreal replication mechanism for replicated fields.
- Prefer `OnRep_` functions for client-side UI/ViewModel updates.
- Do not assume `SetIsReplicated(true)` is enough for nested `UObject` state.
- Check `HasAuthority()` before server-only state mutation.
- Avoid running gameplay-changing logic independently on every client.
- Distinguish clearly between server-only logic, owning-client logic, simulated-proxy logic, and cosmetic-only client logic.
- Do not add session/lobby/OnlineSubsystem logic unless explicitly requested.
- Do not optimize for competitive PvP latency unless explicitly requested.
- Keep replication changes incremental and testable for solo development.

## Attribute system rules

The project currently uses a custom lightweight attribute system rather than GAS.

When editing attributes:

- Preserve separation between:
  - core gameplay state;
  - component ownership;
  - ViewModel/UI-facing data.
- Do not introduce GAS unless explicitly requested.
- Attribute changes should broadcast change events consistently.
- Clamp current values safely.
- Avoid duplicating attribute state across unrelated systems.
- If multiplayer is involved, attribute state should become server-authoritative.
- Do not move attribute logic into UI.
- Do not let ViewModels mutate authoritative gameplay state unless explicitly intended.

## Combat networking rules

For combat implementation:

- Prioritize responsive local feel without giving clients authority over outcomes.
- Local clients may start cosmetic-only attack feedback immediately.
- Server should validate attack requests, stamina/cooldown requirements, hit detection policy, and final damage.
- Replicate confirmed combat results to clients.
- Do not make UI, ViewModels, or client-only code the source of truth for combat state.
- Keep first implementation simple. Avoid lag compensation, rollback, complex reconciliation, or server rewind unless explicitly requested.
- For PvE, prefer server-controlled enemies and replicated enemy state.
- For host player, remember the host is both server and local client; avoid code paths that accidentally work only for the host.

## UI / ViewModel rules

- ViewModels should expose Blueprint-friendly state and delegates.
- UI should observe ViewModels rather than directly mutating core gameplay state.
- Do not bind UI directly to low-level mutable gameplay state unless explicitly intended.
- Avoid adding `UMG`, `Slate`, or widget C++ dependencies unless the task requires actual widget classes in C++.
- Mention manual Unreal Editor steps when Blueprint widgets may need reassignment, recompilation, or binding refresh.

## Input rules

The project uses `EnhancedInput`.

When implementing input:

- Prefer `UInputMappingContext` and `UInputAction`.
- Add mapping context from the local player subsystem.
- Keep input binding in `PlayerController` or `Character` according to ownership intent.
- Do not hardcode gameplay behavior directly in input callbacks if it should belong to character, movement, combat, or ability components.
- For multiplayer gameplay input, route authoritative changes through server-owned logic.

## Build and dependency rules

When adding modules, update `ARPG.Build.cs`.

- Do not add dependencies unnecessarily.
- Do not add plugins or modules without explicit approval unless the task cannot be completed otherwise.
- If a dependency is required, state why it is required.
- Prefer existing project dependencies and patterns.

Common dependencies may include, but should not be added unless needed:

- `EnhancedInput` for input.
- `UMG` for C++ widgets.
- `AIModule` for AI.
- `NavigationSystem` for navigation.
- `GameplayTags` for tag-driven gameplay.
- `GameplayAbilities` only if explicitly requested.
- `NetCore` or replication-related modules only when actually needed.

## Architecture guardrails

- Do not convert existing raw `UPROPERTY` `UObject*` pointers to `TObjectPtr` only for style cleanup.
- Do not add `CoreRedirects` automatically unless a reflected symbol was actually renamed or I explicitly ask for redirects.
- Do not introduce Unreal MVVM plugin/framework, `UMG`, `GAS`, `GameplayTags`, `OnlineSubsystem`, dedicated-server-first architecture, rollback netcode, or peer-to-peer synchronization just because they are common Unreal patterns.
- Prefer the project's current lightweight systems unless I explicitly ask to replace or expand them.

## Code style

- Follow Unreal Engine C++ style.
- Preserve existing naming, folder layout, and file organization.
- Prefer early returns for invalid state.
- Use `nullptr`, not `NULL`.
- Use `FMath` for math helpers.
- Use `UE_LOG` for diagnostics.
- Avoid excessive comments that restate obvious code.
- Comments may be in Russian if matching the existing codebase.
- Prefer clear Unreal-native patterns over generic C++ patterns when Unreal requires them.
- Do not convert code to a different architectural style unless explicitly requested.

## Content and Blueprint safety

- Do not edit `.uasset` or `.umap` files as text.
- Treat Blueprint references to C++ classes, properties, functions, categories, and metadata as potentially fragile.
- Before changing Blueprint-exposed C++ API, identify the likely Blueprint impact.
- If a C++ change may require manual Blueprint action, say so explicitly.
- Prefer additive changes over renames/removals when Blueprint compatibility matters.

## Verification

Do not run expensive verification automatically.

Instead, tell me exactly what to run locally using this format:

```text
Проверка вручную:
1. <command or Unreal Editor action>
2. Что проверить: <expected result>
3. Если упадёт, пришли: <specific output/logs needed>
```

Assume I can manually run builds, tests, lint, typecheck, Unreal Editor startup, PIE, multiplayer PIE, packaging, and bug reproduction steps, then paste the output back.

For Unreal-specific changes, suggest the narrowest useful verification, for example:

- build only the affected target;
- compile from Unreal Editor only if needed;
- run PIE only for gameplay changes;
- run multiplayer PIE only for replication changes;
- recompile affected Blueprint assets only if Blueprint-facing API changed.

## After editing

Return these sections, in Russian:

1. Изменённые файлы.
2. Что изменено.
3. Объяснение для понимания.
4. Проверка не запускалась, если была пропущена.
5. Что запустить вручную.
6. Риски или допущения.

For trivial changes, keep explanations short.

For non-trivial C++, Unreal, Blueprint, gameplay, networking, replication, input, attribute, or architecture changes, the `Объяснение для понимания` section should be detailed enough for me to understand what changed and why.

Do not hide complexity. Explain it clearly and practically.

Do not include unrelated theory, but do explain the concepts directly involved in the change.

Do not perform extra file reads just to make the explanation longer.
