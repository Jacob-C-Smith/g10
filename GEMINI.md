# G10 Codebase Instructions for LLMs

This document provides a high-level overview of key data structures and the rendering system within the G10 codebase. Use this as a guide for understanding and modifying the project.

## Core Data Structures

The core data structures are implemented in C and provide fundamental building blocks for the rest of the engine. They are located in `gsdk/src/`.

### 1. `array` Module

*   **Location**: `gsdk/src/data/array/`
*   **Purpose**: A thread-safe, dynamic array implementation in C. It is used for managing ordered collections of elements.

*   **Key Functionalities**:
    *   **Construction**: `array_construct` creates an array with an initial size. `array_from_elements` and `array_from_arguments` provide other ways to initialize an array.
    *   **Manipulation**: `array_add`, `array_remove`, and `array_set` for modifying the array's contents. The array automatically resizes when its capacity is exceeded.
    *   **Access**: `array_index` allows accessing elements with both positive and negative indices (e.g., `-1` for the last element). `array_slice` can be used to get a sub-array.
    *   **Iteration**: `array_fori` (with index) and `array_foreach` (without index) for iterating over elements.
    *   **Utilities**: `array_sort` for sorting, `array_map` for applying a function to each element, and `array_hash` for computing a hash of the array's contents.
    *   **Serialization**: `array_pack` and `array_unpack` for serializing and deserializing the array.

### 2. `dict` Module

*   **Location**: `gsdk/src/data/dict/`
*   **Purpose**: A thread-safe hash table (dictionary) implementation in C, mapping string keys to `void*` values.

*   **Key Functionalities**:
    *   **Construction**: `dict_construct` creates a dictionary with a specified hash table size.
    *   **Manipulation**: `dict_add` adds or updates a key-value pair. `dict_pop` removes a key-value pair. `dict_clear` removes all items.
    *   **Access**: `dict_get` retrieves a value by its key. `dict_keys` and `dict_values` retrieve iterable lists of all keys and values.
    *   **Iteration**: `dict_foreach` iterates over the values in the dictionary.
    *   **Serialization**: `dict_pack` and `dict_unpack` for serializing and deserializing the dictionary.

### 3. `json` Module

*   **Location**: `gsdk/src/reflection/json/`
*   **Purpose**: A JSON parser and serializer. It uses the `array` and `dict` modules to represent JSON arrays and objects in memory.

*   **Key Functionalities**:
    *   **Parsing**: `json_value_parse` is the primary function for parsing a JSON string into a `json_value` struct. The `json_value` struct is a tagged union that can represent any valid JSON type (object, array, string, number, boolean, null).
    *   **Serialization**: `json_value_serialize` writes a `json_value` to a character buffer. `json_value_fprint` and `json_value_print` write the JSON representation to a file or `stdout`, respectively.
    *   **Memory Management**: `json_value_free` recursively frees the memory allocated for a `json_value` and all its nested values.

## Rendering System

The rendering system is defined by the `renderer` property within the main `assets/instance.json` configuration file. The C code that interprets this JSON structure and drives the rendering using SDL3 is primarily located in `src/g10_sdl3.c`.

### How `g10_sdl3.c` maps to `instance.json`

The `g_sdl3_renderer_from_json` function in `g10_sdl3.c` is the main entry point for parsing the entire `renderer` object from the JSON. It orchestrates the setup of the entire rendering pipeline.

1.  **`renderer.attachments` -> `g_sdl3_attachment_from_json`**:
    *   Each object in the `attachments` array in `instance.json` defines a render target, such as a texture for color, a depth buffer, or the final display framebuffer.
    *   The `g_sdl3_attachment_from_json` function reads the `name` and `type` of each attachment.
    *   Based on the `type` (`texture`, `depth`, `framebuffer`), it creates the corresponding `SDL_GPUTexture` with the correct format and usage flags. A special case is the name "OUTPUT", which is mapped to the window's swapchain texture.

2.  **`renderer.pipelines` -> `g_sdl3_pipeline_from_json`**:
    *   Each object in the `pipelines` array defines a complete graphics pipeline.
    *   `g_sdl3_pipeline_from_json` parses a pipeline definition, including its `name`, `source` shaders (`.metallib` files), vertex `input` layout (e.g., `xyz`, `uv`), `uniforms`, and `samplers`.
    *   It loads and compiles the vertex and fragment shaders, sets up the vertex attribute and buffer descriptions, and creates a fully configured `SDL_GPUGraphicsPipeline`.

3.  **`renderer.passes` -> `g_sdl3_render_pass_from_json`**:
    *   Each object in the `passes` array defines a single render pass.
    *   `g_sdl3_render_pass_from_json` parses a pass definition, which includes a `name`, a `framebuffer` configuration, and a list of `pipelines` to execute within that pass.
    *   It calls `g_sdl3_framebuffer_from_json` to configure the render targets for the pass, referencing the attachments created earlier by name.
    *   It then populates its list of pipelines by looking up the previously created pipelines in a cache.

The overall flow is:
1.  `g_init` calls `g_sdl3_renderer_from_json`.
2.  This function first parses all `attachments` and `pipelines`, caching them by name.
3.  Then, it parses the `passes`, linking the already-cached attachments and pipelines to each pass.
4.  Finally, during the main render loop, `renderer_render` calls `g_sdl3_render_draw`, which iterates through each `render_pass` and executes its associated `pipelines`.
