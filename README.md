Video Showcase: https://www.youtube.com/watch?v=oLLo8pgzysY

Two-Brothers Project is a prototype built in Unreal Engine 5 that demonstrates a modular inventory and ability framework integrated with Unreal’s Gameplay Ability System (GAS). It supports multiplayer replication, where items can modify player attributes, grant new abilities, and update user interfaces across clients. The optional “parasite” subsystem allows dynamic behavioral modifiers that attach to players or items, enabling flexible gameplay experimentation. The goal is to showcase a clean, extensible architecture for inventory, ability, and status systems in networked environments.

Motivation & Goals

In many game systems, inventory, abilities, and stat modifiers are tightly coupled, making them difficult to extend or maintain. This project was created to explore a decoupled and scalable approach:

Items and abilities are modular, so new functionality can be added without rewriting core systems

All gameplay state is replicated correctly across server and clients, minimizing desync risks

The parasite mechanic allows you to experiment with modifiers that dynamically attach and detach, overlaying or altering behavior

The UI is designed to reflect live changes in ability status, cooldowns, and attribute values

This project demonstrates not just what the system does, but also how it is architected for maintainability and multiplayer correctness.

Architecture & System Design

The project is divided into several subsystems, each with clear responsibilities:

Inventory System: Manages item storage, equip/unequip workflows, and stat conversions. It emits signals or events when inventory changes, which trigger updates to the ability system or UI logic.

Ability & Attribute Module using GAS: Uses GAS attribute sets for core stats (health, damage, defense, etc.), and ability specs to manage ability logic, cooldowns, and activation.

Replication and Networking: Each relevant state (inventory contents, ability cooldowns, attribute values) is replicated via Unreal’s replication (RepNotify, RPCs) to ensure clients and server remain in sync.

UI / Feedback Layer: Binds to replicated state so that ability icons, cooldown timers, and stat readouts update immediately in the client’s UI.

The architecture emphasizes decoupling: inventory doesn’t need to know how abilities are implemented, and abilities don’t need to know inventory internals. Changes propagate through event-based or observer-like relationships.

Setup & Running Instructions

Clone the repository:

git clone https://github.com/ethanfernand3s/Two-Brothers-Project.git
cd Two-Brothers-Project


Open the .uproject file in Unreal Engine 5. Allow dependent modules to build if prompted.

In the Unreal Editor, run a multiplayer Play-In-Editor (PIE) session with 2+ clients to test replication.

Use the in-editor UI to equip items, activate abilities, and verify that all clients display consistent state.

Example Usage Scenarios

Equip an item (e.g. “Sword of Strength”) → see the player’s strength attribute increase and UI update accordingly.

Activate a GAS ability (e.g. “Fireball”) → ability triggers, cooldown begins, and effect is applied and replicated.

Project Roadmap & Future Enhancements

Future improvements and extensions might include:

A loot system with randomized item drops or rarity tiers

Procedural creatures and terrain

Evolution system with interchanging bodyparts

Persistent saving of inventory and player stats between sessions

More complex abilities

Automated tests (unit and integration) to validate state transitions and replication correctness

Performance optimization and network message batching for large-scale usage

UI polish and visual feedback enhancements (animations, VFX)

Design Challenges & Considerations

Some of the critical issues addressed or encountered during development include:

Ensuring server-side authority to prevent cheating while allowing responsive client-side prediction

Minimizing replication overhead by batching changes rather than sending every small update

Handling event ordering (e.g., when equip triggers ability availability, which then updates UI)

Designing extension points so future item or ability types can plug in without breaking core systems

Each folder contains module-specific code and internal documentation as needed.

Contributing & Collaboration

While this is primarily a personal/prototype project, contributions are welcome. If you wish to contribute:

Review the coding style and modular architecture before creating changes

Keep changes isolated (e.g. new ability or item modules should not modify core systems)

Document your additions or modifications, especially replication logic or edge cases

License

Include your license of choice (e.g. MIT, Apache 2.0). This section clarifies how others can use, modify, or redistribute your work.

Author & Contact

Dylan Fernandes
Ethan Fernandes
Email: dylanjfernandes06@gmail.com
Email: ethanfernandes0620@gmail.com

GitHub: https://github.com/DylanJFernandes
GitHub: https://github.com/ethanfernand3s
