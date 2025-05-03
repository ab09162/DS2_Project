// File: main_gui.cpp
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
    ProductCatalog catalog;
    catalog.addProduct("Electronics", "Smartphone", 101, 699.99f);
    catalog.addProduct("Electronics", "Smartwatch", 102, 199.99f);
    catalog.addProduct("Electronics", "Smart TV", 103, 1299.00f);
    catalog.addProduct("Books", "Data Structures and Algorithms", 201, 49.99f);
    catalog.addProduct("Books", "Clean Code", 204, 39.99f);

    // UI state
    char searchQuery[256] = "";
    int selectedCategoryIndex = 0;
    std::vector<ProductSuggestion> suggestions;
    std::string feedback;

    // Image & detail state
    bool showDetails = false;
    int   detailID    = 0;
    std::map<int, GLuint>   textureMap;
    std::map<int, ImVec2>   sizeMap;
    std::map<int, std::string> descMap = {
        {101, "A powerful smartphone with OLED display and fast processor."},
        {102, "A sleek smartwatch with fitness tracking and notifications."},
        {103, "A 55-inch Smart TV with 4K resolution and HDR support."},
        {201, "Comprehensive guide to data structures and algorithms."},
        {204, "A handbook of best practices in writing clean, maintainable code."}
    };

    // Paths to images (relative to exe)
    std::map<int, std::string> pathMap = {
        {101, "images/smartphone.png"},
        {102, "images/smartwatch.png"},
        {103, "images/smarttv.png"},
        {201, "images/dsa.png"},
        {204, "images/cleancode.png"}
    };
    // Preload textures
    for (auto& kv : pathMap) {
        int w,h;
        GLuint tex;
        if (LoadTextureFromFile(kv.second.c_str(), &tex, &w, &h)) {
            textureMap[kv.first] = tex;
            sizeMap[kv.first]    = ImVec2((float)w, (float)h);
        }
    }

    // Build category list
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

        // Main Window
        ImGui::Begin("Product Search");
        ImGui::SetNextItemWidth(300);
        ImGui::Combo("Category", &selectedCategoryIndex, catItems.data(), (int)catItems.size());
        ImGui::Separator();
        ImGui::SetNextItemWidth(400);
        ImGui::InputText("Search", searchQuery, sizeof(searchQuery));
        suggestions = catalog.searchProducts(searchQuery, (selectedCategoryIndex>0?cats[selectedCategoryIndex]:""), 10);
        ImGui::Separator();
        ImGui::Text("Suggestions:");
        if (suggestions.empty()) {
            ImGui::Text("No products found matching '%s'", searchQuery);
        } else {
            ImGui::BeginChild("scrolling");
            for (auto& s: suggestions) {
                char buf[256];
                snprintf(buf, sizeof(buf), "%s (ID:%d)  $%.2f", s.name.c_str(), s.productID, s.popularity);
                if (ImGui::Selectable(buf)) {
                    detailID    = s.productID;
                    showDetails = true;
                }
            }
            ImGui::EndChild();
        }
        ImGui::Separator();
        if (!feedback.empty()) ImGui::TextWrapped("%s", feedback.c_str());
        ImGui::End();

        // Details Window
        if (showDetails) {
            ImGui::Begin("Product Details", &showDetails);
            ProductSuggestion info = suggestions.front();
            for (auto& s: suggestions) if (s.productID == detailID) info = s;

            ImGui::Text("%s", info.name.c_str());
            ImGui::Text("Price: $%.2f", info.popularity);
            ImGui::Separator();
            if (textureMap.count(detailID)) {
                ImVec2 sz = sizeMap[detailID];
                float aspect = sz.x > 0 ? sz.y/sz.x : 1.0f;
                ImTextureID tex_id = (ImTextureID)(intptr_t)textureMap[detailID];
                ImGui::Image(tex_id, ImVec2(200, 200 * aspect), ImVec2(0,0), ImVec2(1,1));
            }
            ImGui::Separator();
            if (descMap.count(detailID))
                ImGui::TextWrapped(descMap[detailID].c_str());
            ImGui::End();
        }

        ImGui::Render();
        int w,h; glfwGetFramebufferSize(window, &w, &h);
        glViewport(0,0,w,h);
        glClearColor(0.45f,0.55f,0.60f,1.00f);
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
