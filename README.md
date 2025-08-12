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

| Theme Example | Syntax Highlighting | Editor in Action |
|---------------|--------------------|------------------|
| ![Theme Example](asset/image1.png) | ![Syntax Highlighting](asset/image2.png) | ![Editor in Action](asset/image3.png) |

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

Example theme snippet:
```json
{
    "mainWindow": "#0a0a0a",
    "titlebar": "#0e0e0e",
    "syntaxKeywords": "#ffcc66",
    "syntaxStrings": "#99cc99"
}


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


