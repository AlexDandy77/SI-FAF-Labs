# Lab Report Writer

Generate a complete LaTeX lab report in `docs/lab<N>.tex` for this IoT/embedded systems lab.

## Step 1 — Gather information

Before writing anything, read:
1. All `.h` and `.cpp` files in `src/lab<N>App/` (including subdirectories)
2. `diagram.json` and `wokwi.toml` in that folder (for hardware pin/component details)
3. `platformio.ini` — note the active environment, board, lib_deps, and `APP_NAME` value

If the lab number is ambiguous, ask before proceeding.

## Step 2 — Write the report

Output to `docs/lab<N>.tex`. Do **not** include `\documentclass`, `\begin{document}`, or any preamble — the file is `\input`-ted into a master document.

---

## Report structure

### Chapter 1 — Analysis of the Domain Situation

#### 1.1 Technologies and Application Context
- What the lab implements and on which board (clock, SRAM, Flash)
- Which sensors/actuators/peripherals are used
- Which scheduler variant: bare-metal loop, custom timer-based, or FreeRTOS (with version)
- Which assignment variant is targeted and why it earns the highest grade
- **Key Learning Objectives:** bullet list

#### 1.2 Hardware and Software Components
Nested `\begin{itemize}`. Each top-level bullet is a bold component name + em-dash description. Sub-bullets cover:
- MCU specs
- Each sensor/actuator with pin numbers, power, signal type
- Each output device (LCD I2C address, LED pins + resistor values)
- Framework/RTOS with key APIs used
- Firmware modules with one-line roles each
- Development tools (VSCode, PlatformIO, Wokwi)

#### 1.3 System Architecture and Solution Justification
Layered bullet list describing each architectural layer (HAL → drivers → managers → task layer → app). Then a second list: reasons this architecture was chosen (separation of concerns, scalability, determinism, etc.).

#### 1.4 Relevant Case Study
5–6 real-world domains where the same pattern applies. Close with one sentence on practical relevance.

---

### Chapter 2 — Design

#### 2.1 Architectural Sketch and Component Interconnection
- **Hardware Components:** itemize list (component, pin/address, power)
- Figure placeholder: `\includegraphics[width=18cm]{"images/architecture_diagram.png"}`
- **Component Interfaces:** nested itemize — every public function with signature in `\texttt{}`
- **Interconnection and Data Flow:** numbered `\begin{enumerate}` describing the data path

#### 2.2 Behavior Diagram and Algorithm Description
- Figure placeholder for `images/algorithm_description.png`
- **Initialisation Sequence:** `\begin{verbatim}` numbered startup steps
- One `\begin{verbatim}` pseudocode block per task (LOOP FOREVER / mutex / delay pattern)
- Alert/FSM description with states, thresholds, and debounce count if applicable
- Task priority + timing ASCII table in `\begin{verbatim}`
- Comparison table vs previous lab in `\begin{tabular}{|l|l|l|}` if applicable

#### 2.3 Electrical Schematic
- Figure placeholder for `images/electrical_scheme.pdf`
- Per-peripheral bullet list: voltage, GND, signal pin, resistor values, I2C address

#### 2.4 Project Structure Organisation
`\begin{verbatim}` showing the `src/` tree for this lab. Follow with a bullet list mapping each module to its responsibility.

#### 2.5 Modular Implementation

**Header Files (.h)** — one `\subsubsection` per header; paste actual content in `\begin{verbatim}`.

**Source Files (.cpp)** — one `\subsubsection` per `.cpp`; paste full implementation in `\begin{verbatim}`. After each block add **Implementation Details:** bullet list explaining non-obvious choices.

---

### Chapter 3 — Presentation of Results

#### 3.1 System Interaction Screenshots
- `\begin{verbatim}` blocks: normal state, each alert state, transitions
- Bullet list: LED behaviour per state
- `\begin{verbatim}` for LCD line format
- Figure placeholders: `initial_state`, each alert, `serial_report`

#### 3.2 Simulation Results
- **Wokwi Simulator Testing:** bullet list — one entry per verified behaviour
- **Performance Metrics:** bullet list with concrete numbers (periods, confirmation time, SRAM %, Flash %)

---

### Chapter 4 — Conclusions

#### 4.1 System Performance and Limitations
- **System Performance** subsection: what the system demonstrated
- **Limitations and Considerations** subsection: known constraints

#### 4.2 Laboratory Work Conclusions
Bullet list of achieved goals; last bullet references the assignment variant.

**Key Takeaways** subsection: numbered list of 4–5 engineering lessons specific to this lab.

#### 4.3 Impact in Real Applications
4–5 real domains.

**Future Work and Enhancements** subsection: concrete improvements (new sensors, runtime config, logging, etc.).

---

### Chapter 5 — Note on AI Tool Usage

```latex
\chapter{Note on AI Tool Usage}

During the preparation of this report, I used \textbf{Claude (Anthropic)} for generation of basic LaTeX report structure and some formatting recommendations. I also used \textbf{Eraser AI app} for generating templates of architectural diagrams and flowcharts.

\textbf{Important Note:} All generated information was thoroughly reviewed, validated, and adjusted to accurately reflect the implemented system architecture and actual source code. The technical content, algorithm descriptions, and design decisions are based on direct analysis of the project source files and represent accurate documentation of the implemented solution.
```

---

### Chapter 6 — Bibliography

Numbered `\begin{enumerate}`. Always include:
- MCU datasheet (Microchip/Atmel)
- FreeRTOS book + relevant API pages (if RTOS used)
- Wokwi docs for each simulated component
- PlatformIO docs
- Arduino reference

---

### Chapter 7 — Appendix — Source Code

`\chapter{Appendix -- Source Code}` with one `\section` per source file; full content in `\begin{verbatim}`.

---

## Style rules

- Component names: `\textbf{}` — pin names and identifiers: `\texttt{}`
- Units: non-breaking space before unit (`16~MHz`, `8~KB`, `220~$\Omega$`)
- Temperatures: `$^{\circ}$C`
- All figures: `[H]` float, `width=18cm`, double-quoted image path: `{"images/foo.png"}`
- Pseudocode and code listings: `\begin{verbatim}`
- Tables: `|l|l|l|` style with `\hline` on every row
- File header comment:
  ```
  % =====================================================
  % Lab <N> — <Short title>
  %         <Subtitle>
  % =====================================================
  ```
- Section separator between chapters: `% =====================================================`
- Do not add `\newpage`, `\clearpage`, or `\tableofcontents`
