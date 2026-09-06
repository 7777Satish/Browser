# FluxIDE - A Web Browser Renderer

A lightweight web browser implementation written in C, focusing on core HTML and CSS rendering capabilities.

## Overview

FluxIDE is an experimental browser engine that parses and renders a subset of HTML and CSS standards. The project explores the fundamentals of layout algorithms, styling systems, and graphical rendering in a minimal footprint.

## Screenshots

![FluxIDE Browser Rendering](screenshots/Screenshot%20from%202026-09-06%2018-23-33.png)

## Features

- **HTML Parsing**: Builds a DOM tree structure from HTML markup
- **CSS Styling**: Parses CSS rules and applies styles to DOM elements
- **Layout Engine**: Calculates element positions and dimensions
- **Text Rendering**: Renders text content with typography support
- **Image Support**: Handles image elements with texture management
- **Tab System**: Multi-tab interface for managing pages
- **Mouse Interaction**: Supports scrolling and other mouse events

## Current Capabilities

The engine successfully renders:
- Basic HTML elements (text, images, containers)
- CSS properties (colors, sizing, spacing, typography)
- Layout flows (positioning, margins, padding)
- Simple page compositions with styled content

## Technical Stack

- **Language**: C
- **Graphics**: SDL2, SDL2_gfx, SDL2_image, SDL2_ttf
- **Build System**: Make
- **Architecture**: Modular component design (renderer, parser, layout engine)

## Project Status

🚧 **In Development** — Core rendering pipeline is functional, but the implementation covers only a subset of HTML/CSS standards. Many web features remain unimplemented.

## Building

```bash
make
```

## Running

```bash
./main
```

## Structure

- `src/` - Source files for renderer, HTML/CSS parsers, and utilities
- `include/` - Header files with data structures and function declarations
- `pages/` - Default HTML pages for testing
- `assets/` - Fonts and visual resources
- `build/` - Compiled object files and output

## Future Work

- Expanded CSS property support
- JavaScript execution
- Advanced layout models (Flexbox, Grid)
- Better performance optimization
- More comprehensive HTML element support
