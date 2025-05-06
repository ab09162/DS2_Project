// // File: main_gui.cpp
// #include "ProductCatalog.h" // Your backend logic

// // --- Dependency Headers ---
// #include "glad/glad.h"
// #include <GLFW/glfw3.h>
// #include "imgui.h"
// #include "imgui_impl_glfw.h"
// #include "imgui_impl_opengl3.h"

// // For image loading
// #define STB_IMAGE_IMPLEMENTATION
// #include "stb_image.h"

// #include <iostream>
// #include <vector>
// #include <string>
// #include <map>
// #include <cstdio>

// // --- GLFW Error Callback ---
// static void glfw_error_callback(int error, const char* description) {
//     std::cerr << "GLFW Error " << error << ": " << description << std::endl;
// }

// // Helper: load a texture from file into OpenGL
// bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height) {
//     int width, height, channels;
//     unsigned char* data = stbi_load(filename, &width, &height, &channels, 4);
//     if (!data) return false;
//     glGenTextures(1, out_texture);
//     glBindTexture(GL_TEXTURE_2D, *out_texture);
//     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//     stbi_image_free(data);
//     *out_width  = width;
//     *out_height = height;
//     return true;
// }

// int main(int, char**) {
//     // --- GLFW init ---
//     glfwSetErrorCallback(glfw_error_callback);
//     if (!glfwInit()) return 1;

//     const char* glsl_version = "#version 330 core";
//     glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//     glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//     glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
// #ifdef __APPLE__
//     glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
// #endif

//     GLFWwindow* window = glfwCreateWindow(1280, 720, "Product Catalog", nullptr, nullptr);
//     if (!window) { glfwTerminate(); return 1; }
//     glfwMakeContextCurrent(window);
//     glfwSwapInterval(1);

//     if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return 1;

//     // --- ImGui setup ---
//     IMGUI_CHECKVERSION();
//     ImGui::CreateContext();
//     ImGuiIO& io = ImGui::GetIO();
//     io.FontGlobalScale = 1.5f;
//     ImGuiStyle& style = ImGui::GetStyle();
//     style.FramePadding = ImVec2(8,8);
//     style.ItemSpacing  = ImVec2(12,12);
//     ImGui::StyleColorsDark();
//     ImGui_ImplGlfw_InitForOpenGL(window, true);
//     ImGui_ImplOpenGL3_Init(glsl_version);

//     // --- Prepare Data ---
//     ProductCatalog catalog;
//     catalog.addProduct("Electronics", "Smartphone", 101, 699.99f);
//     catalog.addProduct("Electronics", "Smartwatch", 102, 199.99f);
//     catalog.addProduct("Electronics", "Smart TV", 103, 1299.00f);
//     catalog.addProduct("Books", "Data Structures and Algorithms", 201, 49.99f);
//     catalog.addProduct("Books", "Clean Code", 204, 39.99f);

//     // UI state
//     char searchQuery[256] = "";
//     int selectedCategoryIndex = 0;
//     std::vector<ProductSuggestion> suggestions;
//     std::string feedback;

//     // Image & detail state
//     bool showDetails = false;
//     int   detailID    = 0;
//     std::map<int, GLuint>   textureMap;
//     std::map<int, ImVec2>   sizeMap;
//     std::map<int, std::string> descMap = {
//         {101, "A powerful smartphone with OLED display and fast processor."},
//         {102, "A sleek smartwatch with fitness tracking and notifications."},
//         {103, "A 55-inch Smart TV with 4K resolution and HDR support."},
//         {201, "Comprehensive guide to data structures and algorithms."},
//         {204, "A handbook of best practices in writing clean, maintainable code."}
//     };

//     // Paths to images (relative to exe)
//     std::map<int, std::string> pathMap = {
//         {101, "images/smartphone.png"},
//         {102, "images/smartwatch.png"},
//         {103, "images/smarttv.png"},
//         {201, "images/dsa.png"},
//         {204, "images/cleancode.png"}
//     };
//     // Preload textures
//     for (auto& kv : pathMap) {
//         int w,h;
//         GLuint tex;
//         if (LoadTextureFromFile(kv.second.c_str(), &tex, &w, &h)) {
//             textureMap[kv.first] = tex;
//             sizeMap[kv.first]    = ImVec2((float)w, (float)h);
//         }
//     }

//     // Build category list
//     std::vector<std::string> cats = {"All Categories"};
//     auto backendCats = catalog.getCategories();
//     cats.insert(cats.end(), backendCats.begin(), backendCats.end());
//     std::vector<const char*> catItems;
//     for (auto& c: cats) catItems.push_back(c.c_str());

//     // --- Main Loop ---
//     while (!glfwWindowShouldClose(window)) {
//         glfwPollEvents();
//         ImGui_ImplOpenGL3_NewFrame();
//         ImGui_ImplGlfw_NewFrame();
//         ImGui::NewFrame();
        
//         // Main Window
//         ImGui::Begin("Product Search");
//         ImGui::SetNextItemWidth(300);
//         ImGui::Combo("Category", &selectedCategoryIndex, catItems.data(), (int)catItems.size());
//         ImGui::Separator();
//         ImGui::SetNextItemWidth(400);
//         ImGui::InputText("Search", searchQuery, sizeof(searchQuery));
//         suggestions = catalog.searchProducts(searchQuery, (selectedCategoryIndex>0?cats[selectedCategoryIndex]:""), 10);
//         ImGui::Separator();
//         ImGui::Text("Suggestions:");
//         if (suggestions.empty()) {
//             ImGui::Text("No products found matching '%s'", searchQuery);
//         } else {
//             ImGui::BeginChild("scrolling");
//             // for (auto& s: suggestions) {
//             //     char buf[256];
//             //     snprintf(buf, sizeof(buf), "%s (ID:%d)  $%.2f", s.name.c_str(), s.productID, s.popularity);
//             //     if (ImGui::Selectable(buf)) {
//             //         detailID    = s.productID;
//             //         showDetails = true;
//             //     }
//             // }
//             for (auto& s: suggestions) {
//                 char buf[256];
//                 snprintf(buf, sizeof(buf), "%s (ID:%d)  $%.2f", s.name.c_str(), s.productID, s.popularity);
//                 ImGui::Selectable(buf);
                
//                 if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
//                     detailID = s.productID;
//                     showDetails = true;
//                 }
//             }

//             ImGui::EndChild();
//         }
//         ImGui::Separator();
//         if (!feedback.empty()) ImGui::TextWrapped("%s", feedback.c_str());
//         ImGui::End();

//         // Details Window
//         if (showDetails) {
//             ImGui::Begin("Product Details", &showDetails);
//             ProductSuggestion info = suggestions.front();
//             for (auto& s: suggestions) if (s.productID == detailID) info = s;

//             ImGui::Text("%s", info.name.c_str());
//             ImGui::Text("Price: $%.2f", info.popularity);
//             ImGui::Separator();
//             if (textureMap.count(detailID)) {
//                 ImVec2 sz = sizeMap[detailID];
//                 float aspect = sz.x > 0 ? sz.y/sz.x : 1.0f;
//                 ImTextureID tex_id = (ImTextureID)(intptr_t)textureMap[detailID];
//                 ImGui::Image(tex_id, ImVec2(200, 200 * aspect), ImVec2(0,0), ImVec2(1,1));
//             }
//             ImGui::Separator();
//             if (descMap.count(detailID))
//                 ImGui::TextWrapped(descMap[detailID].c_str());
//             ImGui::End();
//         }

//         ImGui::Render();
//         int w,h; glfwGetFramebufferSize(window, &w, &h);
//         glViewport(0,0,w,h);
//         glClearColor(0.45f,0.55f,0.60f,1.00f);
//         glClear(GL_COLOR_BUFFER_BIT);
//         ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
//         glfwSwapBuffers(window);
//     }

//     // Cleanup
//     ImGui_ImplOpenGL3_Shutdown();
//     ImGui_ImplGlfw_Shutdown();
//     ImGui::DestroyContext();
//     glfwDestroyWindow(window);
//     glfwTerminate();
//     return 0;
// }
// // File: main_gui.cpp
// #include "ProductCatalog.h" // Your backend logic

// // --- Dependency Headers ---
// #include "glad/glad.h"
// #include <GLFW/glfw3.h>
// #include "imgui.h"
// #include "imgui_impl_glfw.h"
// #include "imgui_impl_opengl3.h"

// // For image loading
// #define STB_IMAGE_IMPLEMENTATION
// #include "stb_image.h"

// #include <iostream>
// #include <vector>
// #include <string>
// #include <map>
// #include <cstdio>

// // --- GLFW Error Callback ---
// static void glfw_error_callback(int error, const char* description) {
//     std::cerr << "GLFW Error " << error << ": " << description << std::endl;
// }

// // Helper: load a texture from file into OpenGL
// bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height) {
//     int width, height, channels;
//     unsigned char* data = stbi_load(filename, &width, &height, &channels, 4);
//     if (!data) return false;
//     glGenTextures(1, out_texture);
//     glBindTexture(GL_TEXTURE_2D, *out_texture);
//     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//     stbi_image_free(data);
//     *out_width  = width;
//     *out_height = height;
//     return true;
// }

// int main(int, char**) {
//     // --- GLFW init ---
//     glfwSetErrorCallback(glfw_error_callback);
//     if (!glfwInit()) return 1;

//     const char* glsl_version = "#version 330 core";
//     glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//     glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//     glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
// #ifdef __APPLE__
//     glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
// #endif

//     GLFWwindow* window = glfwCreateWindow(1280, 720, "Product Catalog", nullptr, nullptr);
//     if (!window) { glfwTerminate(); return 1; }
//     glfwMakeContextCurrent(window);
//     glfwSwapInterval(1);

//     if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return 1;

//     // --- ImGui setup ---
//     IMGUI_CHECKVERSION();
//     ImGui::CreateContext();
//     ImGuiIO& io = ImGui::GetIO();
//     io.FontGlobalScale = 1.5f;
//     ImGuiStyle& style = ImGui::GetStyle();
//     style.FramePadding = ImVec2(8,8);
//     style.ItemSpacing  = ImVec2(12,12);
//     ImGui::StyleColorsDark();
//     ImGui_ImplGlfw_InitForOpenGL(window, true);
//     ImGui_ImplOpenGL3_Init(glsl_version);

//     // --- Prepare Data ---
//     ProductCatalog catalog;
//     catalog.addProduct("Electronics", "Smartphone", 101, 699.99f);
//     catalog.addProduct("Electronics", "Smartwatch", 102, 199.99f);
//     catalog.addProduct("Electronics", "Smart TV", 103, 1299.00f);
//     catalog.addProduct("Books", "Data Structures and Algorithms", 201, 49.99f);
//     catalog.addProduct("Books", "Clean Code", 204, 39.99f);

//     char searchQuery[256] = "";
//     int selectedCategoryIndex = 0;
//     std::vector<ProductSuggestion> suggestions;
//     std::string feedback;

//     bool showDetails = false;
//     int detailID = 0;
//     std::map<int, GLuint> textureMap;
//     std::map<int, ImVec2> sizeMap;
//     std::map<int, std::string> descMap = {
//         {101, "A powerful smartphone with OLED display and fast processor."},
//         {102, "A sleek smartwatch with fitness tracking and notifications."},
//         {103, "A 55-inch Smart TV with 4K resolution and HDR support."},
//         {201, "Comprehensive guide to data structures and algorithms."},
//         {204, "A handbook of best practices in writing clean, maintainable code."}
//     };

//     std::map<int, std::string> pathMap = {
//         {101, "images/smartphone.png"},
//         {102, "images/smartwatch.png"},
//         {103, "images/smarttv.png"},
//         {201, "images/dsa.png"},
//         {204, "images/cleancode.png"}
//     };
//     for (auto& kv : pathMap) {
//         int w, h;
//         GLuint tex;
//         if (LoadTextureFromFile(kv.second.c_str(), &tex, &w, &h)) {
//             textureMap[kv.first] = tex;
//             sizeMap[kv.first] = ImVec2((float)w, (float)h);
//         }
//     }

//     std::vector<std::string> cats = {"All Categories"};
//     auto backendCats = catalog.getCategories();
//     cats.insert(cats.end(), backendCats.begin(), backendCats.end());
//     std::vector<const char*> catItems;
//     for (auto& c: cats) catItems.push_back(c.c_str());

//     while (!glfwWindowShouldClose(window)) {
//         glfwPollEvents();
//         ImGui_ImplOpenGL3_NewFrame();
//         ImGui_ImplGlfw_NewFrame();
//         ImGui::NewFrame();

//         if (!showDetails) {
//             ImGui::Begin("Product Search");
//             ImGui::SetNextItemWidth(300);
//             ImGui::Combo("Category", &selectedCategoryIndex, catItems.data(), (int)catItems.size());
//             ImGui::Separator();
//             ImGui::SetNextItemWidth(400);
//             ImGui::InputText("Search", searchQuery, sizeof(searchQuery));
//             suggestions = catalog.searchProducts(searchQuery, (selectedCategoryIndex > 0 ? cats[selectedCategoryIndex] : ""), 10);
//             ImGui::Separator();
//             ImGui::Text("Suggestions:");
//             if (suggestions.empty()) {
//                 ImGui::Text("No products found matching '%s'", searchQuery);
//             } else {
//                 ImGui::BeginChild("scrolling");
//                 for (auto& s : suggestions) {
//                     char buf[256];
//                     snprintf(buf, sizeof(buf), "%s (ID:%d)  $%.2f", s.name.c_str(), s.productID, s.popularity);
//                     if (ImGui::Selectable(buf)) {
//                         detailID = s.productID;
//                     }
//                     if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
//                         detailID = s.productID;
//                         showDetails = true;
//                     }
//                 }
//                 ImGui::EndChild();
//             }
//             ImGui::End();
//         } else {
//             // ImGui::Begin("Product Details");
//             ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
//             ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize, ImGuiCond_Always);
//             ImGui::Begin("Product Details", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);


//             ProductSuggestion info;
//             for (auto& s : suggestions)
//                 if (s.productID == detailID)
//                     info = s;

//             ImGui::Text("%s", info.name.c_str());
//             ImGui::Text("Price: $%.2f", info.popularity);
//             ImGui::Separator();

//             if (textureMap.count(detailID)) {
//                 ImVec2 sz = sizeMap[detailID];
//                 float aspect = sz.x > 0 ? sz.y / sz.x : 1.0f;
//                 ImTextureID tex_id = (ImTextureID)(intptr_t)textureMap[detailID];
//                 ImGui::Image(tex_id, ImVec2(200, 200 * aspect), ImVec2(0, 0), ImVec2(1, 1));
//             }
//             ImGui::Separator();
//             if (descMap.count(detailID))
//                 ImGui::TextWrapped("%s", descMap[detailID].c_str());

//             if (ImGui::Button("Back")) {
//                 showDetails = false;
//             }

//             ImGui::End();
//         }

//         ImGui::Render();
//         int w, h; glfwGetFramebufferSize(window, &w, &h);
//         glViewport(0, 0, w, h);
//         glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
//         glClear(GL_COLOR_BUFFER_BIT);
//         ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
//         glfwSwapBuffers(window);
//     }

//     ImGui_ImplOpenGL3_Shutdown();
//     ImGui_ImplGlfw_Shutdown();
//     ImGui::DestroyContext();
//     glfwDestroyWindow(window);
//     glfwTerminate();
//     return 0;
// }
// #include "ProductCatalog.h"

// // --- Dependencies ---
// #include "glad/glad.h"
// #include <GLFW/glfw3.h>
// #include "imgui.h"
// #include "imgui_impl_glfw.h"
// #include "imgui_impl_opengl3.h"

// // Image loading
// #define STB_IMAGE_IMPLEMENTATION
// #include "stb_image.h"

// #include <iostream>
// #include <vector>
// #include <string>
// #include <map>

// // GLFW error callback
// static void glfw_error_callback(int error, const char* description) {
//     std::cerr << "GLFW Error " << error << ": " << description << std::endl;
// }

// // Load texture from file using stb_image
// bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height) {
//     int width, height, channels;
//     unsigned char* image_data = stbi_load(filename, &width, &height, &channels, STBI_rgb_alpha);
//     if (!image_data) {

//         std::cerr << "Failed to load image: " << filename << std::endl;
//         std::cerr << "stbi_error: " << stbi_failure_reason() << std::endl; // Prints the reason for failure

//         return false;
//     }

//     GLuint image_texture;
//     glGenTextures(1, &image_texture);
//     glBindTexture(GL_TEXTURE_2D, image_texture);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
//     stbi_image_free(image_data);

//     *out_texture = image_texture;
//     *out_width = width;
//     *out_height = height;

//     std::cout << "Loaded image: " << filename << " (" << width << "x" << height << ")" << std::endl;
//     return true;
// }

// int main(int, char**) {
//     glfwSetErrorCallback(glfw_error_callback);
//     if (!glfwInit()) return 1;

//     const char* glsl_version = "#version 330 core";
//     glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//     glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//     glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
// #ifdef __APPLE__
//     glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
// #endif

//     GLFWwindow* window = glfwCreateWindow(1280, 720, "Product Catalog", nullptr, nullptr);
//     if (!window) { glfwTerminate(); return 1; }
//     glfwMakeContextCurrent(window);
//     glfwSwapInterval(1);
//     if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return 1;

//     // ImGui setup
//     IMGUI_CHECKVERSION();
//     ImGui::CreateContext();
//     ImGuiIO& io = ImGui::GetIO(); (void)io;
//     io.FontGlobalScale = 1.5f;
//     ImGui::StyleColorsDark();
//     ImGui_ImplGlfw_InitForOpenGL(window, true);
//     ImGui_ImplOpenGL3_Init(glsl_version);

//     // Backend product data
//     ProductCatalog catalog;
    //  catalog.addProduct("Electronics", "Smartphones", "iPhone 12", 101, 699.99f);
        // catalog.addProduct("Electronics", "Smartphones", "Samsung Galaxy S21", 102, 799.99f);
        // catalog.addProduct("Electronics", "Smartwatches", "Apple Watch", 103, 399.99f);
        // catalog.addProduct("Books", "Programming", "Clean Code", 201, 49.99f);
        // catalog.addProduct("Books", "Programming", "Algorithms Unlocked", 202, 39.99f);
        // catalog.addProduct("Books", "Design", "Design Patterns", 203, 29.99f);

//     char searchQuery[256] = "";
//     int selectedCategoryIndex = 0;
//     std::vector<ProductSuggestion> suggestions;
//     std::string feedback;
//     bool showDetails = false;
//     int detailID = 0;

//     std::map<int, GLuint> textureMap;
//     std::map<int, ImVec2> sizeMap;

//     std::map<int, std::string> descMap = {
//         {101, "A powerful smartphone with OLED display and fast processor."},
//         {102, "A sleek smartwatch with fitness tracking and notifications."},
//         {103, "A 55-inch Smart TV with 4K resolution and HDR support."},
//         {201, "Comprehensive guide to data structures and algorithms."},
//         {204, "A handbook of best practices in writing clean, maintainable code."}
//     };

//     std::map<int, std::string> pathMap = {
//         {101, "/Users/test/Downloads/DS 2 project/DS2_Project-1/DS2_Proj/images/smartphone.png"},
//         {102, "images/smartwatch.png"},
//         {103, "images/smarttv.png"},
//         {201, "images/dsa.png"},
//         {204, "images/cleancode.png"}
//     };

//     for (auto& kv : pathMap) {
//         int w, h;
//         GLuint tex;
//         if (LoadTextureFromFile(kv.second.c_str(), &tex, &w, &h)) {
//             textureMap[kv.first] = tex;
//             sizeMap[kv.first] = ImVec2((float)w, (float)h);
//         } else {
//             std::cerr << "Failed to load texture for ID " << kv.first << ": " << kv.second << std::endl;
//         }
//     }

//     std::vector<std::string> cats = {"All Categories"};
//     auto backendCats = catalog.getCategories();
//     cats.insert(cats.end(), backendCats.begin(), backendCats.end());
//     std::vector<const char*> catItems;
//     for (auto& c: cats) catItems.push_back(c.c_str());

//     // GUI Loop
//     while (!glfwWindowShouldClose(window)) {
//         glfwPollEvents();
//         ImGui_ImplOpenGL3_NewFrame();
//         ImGui_ImplGlfw_NewFrame();
//         ImGui::NewFrame();

//         if (!showDetails) {
//             ImGui::Begin("Product Search");
//             ImGui::SetNextItemWidth(300);
//             ImGui::Combo("Category", &selectedCategoryIndex, catItems.data(), (int)catItems.size());
//             ImGui::Separator();
//             ImGui::SetNextItemWidth(400);
//             ImGui::InputText("Search", searchQuery, sizeof(searchQuery));
//             suggestions = catalog.searchProducts(searchQuery, (selectedCategoryIndex > 0 ? cats[selectedCategoryIndex] : ""), 10);
//             ImGui::Separator();
//             ImGui::Text("Suggestions:");
//             if (suggestions.empty()) {
//                 ImGui::Text("No products found matching '%s'", searchQuery);
//             } else {
//                 ImGui::BeginChild("scrolling");
//                 for (auto& s : suggestions) {
//                     char buf[256];
//                     snprintf(buf, sizeof(buf), "%s (ID:%d)  $%.2f", s.name.c_str(), s.productID, s.popularity);
//                     if (ImGui::Selectable(buf)) {
//                         detailID = s.productID;
//                     }
//                     if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
//                         detailID = s.productID;
//                         showDetails = true;
//                     }
//                 }
//                 ImGui::EndChild();
//             }
//             ImGui::End();
//         } else {
//             ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
//             ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize, ImGuiCond_Always);
//             ImGui::Begin("Product Details", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

//             ProductSuggestion info;
//             for (auto& s : suggestions)
//                 if (s.productID == detailID)
//                     info = s;

//             ImGui::Text("%s", info.name.c_str());
//             ImGui::Text("Price: $%.2f", info.popularity);
//             ImGui::Separator();

//             if (textureMap.count(detailID)) {
//                 GLuint tex = textureMap[detailID];
//                 ImVec2 sz = sizeMap[detailID];
//                 float aspect = sz.x > 0 ? sz.y / sz.x : 1.0f;
//                 ImGui::Image((ImTextureID)(intptr_t)tex, ImVec2(200, 200 * aspect));
//             } else {
//                 ImGui::TextColored(ImVec4(1, 0, 0, 1), "Image not available.");
//             }

//             ImGui::Separator();
//             if (descMap.count(detailID))
//                 ImGui::TextWrapped("%s", descMap[detailID].c_str());

//             if (ImGui::Button("Back")) {
//                 showDetails = false;
//             }

//             ImGui::End();
//         }

//         ImGui::Render();
//         int w, h; glfwGetFramebufferSize(window, &w, &h);
//         glViewport(0, 0, w, h);
//         glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
//         glClear(GL_COLOR_BUFFER_BIT);
//         ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
//         glfwSwapBuffers(window);
//     }

//     // Cleanup
//     for (auto& kv : textureMap)
//         glDeleteTextures(1, &kv.second);
//     ImGui_ImplOpenGL3_Shutdown();
//     ImGui_ImplGlfw_Shutdown();
//     ImGui::DestroyContext();
//     glfwDestroyWindow(window);
//     glfwTerminate();
//     return 0;
// }
#include "ProductCatalog.h" // Your backend logic

// --- Dependency Headers ---
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

// For image loading
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <cstdio>

// --- GLFW Error Callback ---
static void glfw_error_callback(int error, const char* description) {
    std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}

// Helper: load a texture from file into OpenGL
bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height) {
    int width, height, channels;
    unsigned char* data = stbi_load(filename, &width, &height, &channels, 4);
    if (!data) return false;
    glGenTextures(1, out_texture);
    glBindTexture(GL_TEXTURE_2D, *out_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_image_free(data);
    *out_width  = width;
    *out_height = height;
    return true;
}
#include <fstream>
#include <sstream>

void LoadProductsFromFile(
    const std::string& filename,
    ProductCatalog& catalog,
    std::map<int, std::string>& descMap,
    std::map<int, std::string>& pathMap
) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string category, subcategory, name, idStr, priceStr, imagePath, description;

        if (std::getline(ss, category, '|') &&
            std::getline(ss, subcategory, '|') &&
            std::getline(ss, name, '|') &&
            std::getline(ss, idStr, '|') &&
            std::getline(ss, priceStr, '|') &&
            std::getline(ss, imagePath, '|') &&
            std::getline(ss, description)) {

            int id = std::stoi(idStr);
            float price = std::stof(priceStr);

            catalog.addProduct(category, subcategory, name, id, price);
            pathMap[id] = imagePath;
            descMap[id] = description;
        }
    }

    file.close();
}

int main(int, char**) {
    // --- GLFW init ---
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) return 1;

    const char* glsl_version = "#version 330 core";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(1280, 720, "Product Catalog", nullptr, nullptr);
    if (!window) { glfwTerminate(); return 1; }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return 1;

    // --- ImGui setup ---
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.FontGlobalScale = 1.5f;
    ImGuiStyle& style = ImGui::GetStyle();
    style.FramePadding = ImVec2(8,8);
    style.ItemSpacing  = ImVec2(12,12);
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // --- Prepare Data ---
    std::map<int, std::string> descMap;
    std::map<int, std::string> pathMap;
    ProductCatalog catalog;
    LoadProductsFromFile("product_data.txt", catalog, descMap, pathMap);


    // UI state
    char searchQuery[256] = "";
    int selectedCategoryIndex = 0;
    int selectedSubcategoryIndex = 0;
    std::vector<ProductSuggestion> suggestions;
    std::string feedback;

    // View state
    bool inDetailsView = false;
    int detailID = 0;

    std::map<int, GLuint> textureMap;
    std::map<int, ImVec2> sizeMap;
    // std::map<int, std::string> descMap = {
    //     {101, "A powerful smartphone with OLED display and fast processor."},
    //     {102, "A fast smartphone with AMOLED display and 5G connectivity."},
    //     {103, "A sleek smartwatch with fitness tracking and notifications."},
    //     {201, "A comprehensive guide to writing clean code."},
    //     {202, "A book that unlocks the secrets of algorithms."},
    //     {203, "A book on classic design patterns in software."}
    // };

    // std::map<int, std::string> pathMap = {
    //     {101, "images/smartphone.png"},
    //     {102, "images/smartphone.png"},
    //     {103, "images/smartwatch.png"},
    //     {201, "images/cleancode.png"},
    //     {202, "images/algorithms.png"},
    //     {203, "images/designpatterns.png"}
    // };

    // Preload textures
    for (auto& kv : pathMap) {
        int w, h;
        GLuint tex;
        if (LoadTextureFromFile(kv.second.c_str(), &tex, &w, &h)) {
            textureMap[kv.first] = tex;
            sizeMap[kv.first] = ImVec2((float)w, (float)h);
        }
    }

    std::vector<std::string> cats = {"All Categories"};
    auto backendCats = catalog.getCategories();
    cats.insert(cats.end(), backendCats.begin(), backendCats.end());
    std::vector<const char*> catItems;
    for (auto& c: cats) catItems.push_back(c.c_str());

    // --- Main Loop ---
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Main Product Search Window
        if (!inDetailsView) {
            ImGui::Begin("Product Search");

            ImGui::SetNextItemWidth(300);
            ImGui::Combo("Category", &selectedCategoryIndex, catItems.data(), (int)catItems.size());

            std::vector<std::string> subcategories;
            if (selectedCategoryIndex > 0) {
                std::string selectedCategory = cats[selectedCategoryIndex];
                subcategories = catalog.getSubcategoriesForCategory(selectedCategory);
            }
            subcategories.insert(subcategories.begin(), "All Subcategories");

            const char* subcategoryItems[subcategories.size()];
            for (size_t i = 0; i < subcategories.size(); ++i) {
                subcategoryItems[i] = subcategories[i].c_str();
            }
            ImGui::SetNextItemWidth(300);
            ImGui::Combo("Subcategory", &selectedSubcategoryIndex, subcategoryItems, (int)subcategories.size());

            ImGui::Separator();

            ImGui::SetNextItemWidth(400);
            ImGui::InputText("Search", searchQuery, sizeof(searchQuery));

            // Perform the product search
            if (selectedSubcategoryIndex == 0) {
                suggestions = catalog.searchProducts(searchQuery, 
                    (selectedCategoryIndex > 0 ? cats[selectedCategoryIndex] : ""),
                    "", 
                    10);
            } else {
                suggestions = catalog.searchProducts(searchQuery, 
                    (selectedCategoryIndex > 0 ? cats[selectedCategoryIndex] : ""),
                    subcategories[selectedSubcategoryIndex], 
                    10);
            }

            ImGui::Separator();
            ImGui::Text("Suggestions:");
            if (suggestions.empty()) {
                ImGui::Text("No products found matching '%s'", searchQuery);
            } else {
                ImGui::BeginChild("scrolling");
                for (auto& s : suggestions) {
                    char buf[256];
                    snprintf(buf, sizeof(buf), "%s (ID:%d)  $%.2f", s.name.c_str(), s.productID, s.popularity);
                    ImGui::Selectable(buf);

                    if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
                        detailID = s.productID;
                        inDetailsView = true;
                    }
                }
                ImGui::EndChild();
            }
            ImGui::Separator();
            if (!feedback.empty()) ImGui::TextWrapped("%s", feedback.c_str());
            ImGui::End();
        }

        // Product Details View
        if (inDetailsView) {
            ImGui::Begin("Product Details");

            ProductSuggestion info = suggestions.front();
            for (auto& s : suggestions) {
                if (s.productID == detailID) {
                    info = s;
                    break;
                }
            }

            ImGui::Text("%s", info.name.c_str());
            ImGui::Text("Price: $%.2f", info.popularity);
            ImGui::Separator();

            if (textureMap.count(detailID)) {
                ImVec2 sz = sizeMap[detailID];
                float aspect = sz.x > 0 ? sz.y / sz.x : 1.0f;
                ImTextureID tex_id = (ImTextureID)(intptr_t)textureMap[detailID];
                ImGui::Image(tex_id, ImVec2(200, 200 * aspect));
            }

            ImGui::Separator();
            if (descMap.count(detailID))
                ImGui::TextWrapped(descMap[detailID].c_str());

            ImGui::Separator();
            if (ImGui::Button("Back")) {
                inDetailsView = false;
            }

            ImGui::End();
        }

        // Render Frame
        ImGui::Render();
        int w, h;
        glfwGetFramebufferSize(window, &w, &h);
        glViewport(0, 0, w, h);
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}




