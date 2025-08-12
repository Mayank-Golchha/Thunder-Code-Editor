# Thunder Code Editor

Thunder Code Editor is a **lightweight, blazing-fast code editor** built entirely in **C++** using the **Win32 API** and **GDI+** for rendering.  
It’s designed for speed, extensibility, and customization — with support for themes, syntax highlighting, and an extension API.  
Currently **under development**, but already packed with features that make it stand out.

---

## 🚀 Features

- **Custom Win32 & GDI+ Rendering**  
  No heavy frameworks — the UI is handcrafted using raw Windows API calls and GDI+ for crisp text rendering.

- **Custom JSON Parser**  
  The editor includes its own **JSON parsing engine** for lightning-fast configuration and theme loading.

- **Custom Python Parser**  
  A custom parser for Python syntax is built-in, enabling efficient and accurate syntax highlighting.

- **Syntax Highlighting**  
  Highlight code in multiple languages with customizable color schemes.

- **Multiple Themes**  
  Supports a variety of themes through JSON-based configuration files.

- **File Explorer**
  - Navigate project directories directly within the editor.
  - Smooth expand/collapse animations.

- **Cool Animations**
  - Smooth animations on clicking the editor.
  - Smooth animation on clicking the file explorer.

- **Mini Code Viewer** for quick peeks at file contents.

- **Extension API**  
  Developers can create extensions to add new functionality, language support, or custom commands.

- **Lightweight and Fast**  
  Designed with performance in mind — minimal memory usage, instant load times.

---

## 🛠️ Technologies Used

- **C++** — Core logic and performance-critical features.
- **Win32 API** — For creating and managing native Windows application windows.
- **GDI+** — For drawing text, UI elements, and handling colors.
- **Custom JSON Parser** — Written from scratch in C++ for high-speed configuration parsing.
- **Custom Python Parser** — For Python syntax analysis and highlighting.

---

## 📸 Screenshots

### Theme Example
![Theme Example](asset/image1.png)

### Syntax Highlighting
![Syntax Highlighting](asset/image2.png)

### Editor in Action
![Editor in Action](asset/image3.png)
---

## 📦 Extension API

Thunder comes with an **Extension API** that allows developers to:
- Add **new language parsers**
- Modify **syntax highlighting rules**
- Add **custom commands**
- Create **theme packs**

Extensions are loaded dynamically and can interact with the editor’s core.

---

## 🎨 Themes

Themes are **JSON-based** and control:
- Background colors
- Syntax colors
- UI element styling

Example theme customizing:
```json
{
  "mainWindow": "#0a0a0a",
  "titlebar": "#1a1a2e",
  "windowControlButtonsbk": "#0d1117",
  "windowControlButtonstc": "#44d9ff",
  "hoverButtonColor": "#00c3ff",
  "hoverWindowColorbk": "#11111b",
  "hoverWindowColortc": "#00eaff",

  "menucolorbk": "#0e0e12",
  "menucolortc": "#00c7ff",
  "menucolorhoverbk": "#1a1c2c",
  "menucolorhovertc": "#2fe3ff",

  "hoverwindowhighlightline": "#00ffff",

  "filetabContainer": "#12141c",
  "filetabbk": "#0f0f0f",
  "filetabtc": "#26c9ff",
  "filetabhoverbk": "#1a1f2e",
  "filetabhovertc": "#55f2ff",

  "scrollbarbk": "#131313",
  "scrollbarthumb": "#2fd5ff",

  "editorbk": "#0a0f15",
  "editortc": "#c0dfff",
  "editorSelectedLinebk": "#162030",
  "editorSelectedTextbk": "#2fb9ff",

  "autoCompleteOptionbk": "#10151d",
  "autoCompleteOptiontc": "#37ccff",
  "autoCompleteOptionHoverbk": "#0e1723",
  "autoCompleteOptionHovertc": "#60e7ff",

  "miniCodeViewerbk": "#0a0f18",
  "miniCodeViewertc": "#00b7ff",

  "fileExplorerBk": "#0d1015",
  "fileExplorerItemBk": "#0d1015",
  "fileExplorerItemTc": "#31cfff",
  "fileExplorerHoverItemBk": "#1a232e",
  "fileExplorerHoverItemTc": "#00f0ff",
  "fileExplorerItemSelectedBk": "#13202d",
  "fileExplorerItemSelectedTc": "#68efff",

  "inputWindowBk": "#10151c",
  "inputWindowTc": "#45dbff",

  "contextMenuBk": "#11151c",
  "contextMenuTc": "#44ceff",
  "contextMenuHighLightBk": "#1d2c3c",
  "contextMenuHighLightTc": "#8ff6ff"
}
```

---

## 🚧 Development Status

Thunder Code Editor is **under active development**.  
Some planned features include:
- Multi-tab editing.
- More language parsers.
- Plugin marketplace.
- Advanced debugging tools.

---

## 📜 License

This project is licensed under the [MIT License](LICENSE).

---

## 💡 Contributing

Contributions are welcome!  
- Fork the repository.
- Create your feature branch (`git checkout -b feature/AmazingFeature`).
- Commit your changes (`git commit -m 'Add some AmazingFeature'`).
- Push to the branch (`git push origin feature/AmazingFeature`).
- Open a pull request.

---

## 📬 Contact

**Author:** Mayank Golchha  
**GitHub:** [Mayank-Golchha](https://github.com/Mayank-Golchha)  
**Repo:** [Thunder Code Editor](https://github.com/Mayank-Golchha/Thunder-Code-Editor)


