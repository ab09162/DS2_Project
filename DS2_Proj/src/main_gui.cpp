#include "ProductCatalog.h" // Your backend logic

// --- Dependency Headers ---
// GLAD (needs to be included before GLFW) - Loads OpenGL functions
#include "glad/glad.h"
// GLFW (Windowing and Input)
#include <GLFW/glfw3.h>
// ImGui Core + Backends
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

// Standard Libraries
#include <iostream>
#include <vector>
#include <string>
#include <cstdio> // For sprintf (safe version is better if available)

// --- GLFW Error Callback ---
static void glfw_error_callback(int error, const char* description) {
    std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}

int main(int, char**) {
    // --- 1. Initialize GLFW ---
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return 1;
    }

    // --- 2. Set OpenGL Version and Core Profile ---
    // Request OpenGL 3.3 Core context (adjust if needed, matches GLAD)
    const char* glsl_version = "#version 330 core"; // Set GLSL version string for ImGui
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Required on macOS
#endif

    // --- 3. Create Window ---
    GLFWwindow* window = glfwCreateWindow(1280, 720, "E-Commerce Search Demo (ImGui)", nullptr, nullptr);
    if (window == nullptr) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // --- 4. Initialize GLAD ---
    // Load OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }
    std::cout << "OpenGL Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "OpenGL Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;


    // --- 5. Setup ImGui Context ---
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io; // Basic config
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

    // Setup Dear ImGui style (optional: Dark, Light, Classic)
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);


    // --- 6. Prepare Your Application Data ---
    ProductCatalog catalog;
    // Add sample data (reuse from previous main.cpp or load from file)
    catalog.addProduct("Electronics", "Smartphone", 101, 15.0);
    catalog.addProduct("Electronics", "Smartwatch", 102, 12.0);
    catalog.addProduct("Electronics", "Smart TV", 103, 10.0);
    catalog.addProduct("Electronics", "Laptop", 104, 20.0);
    catalog.addProduct("Electronics", "Wireless Earbuds", 105, 8.0);
    catalog.addProduct("Books", "Data Structures and Algorithms", 201, 5.0);
    catalog.addProduct("Books", "Clean Code", 204, 3.0);
    catalog.addProduct("Clothing", "Smart T-Shirt", 301, 2.0);
    catalog.addProduct("Clothing", "Running Shoes", 302, 6.0);

    // State variables for the UI
    char searchQuery[256] = ""; // Input buffer for search text
    int selectedCategoryIndex = 0; // Index for the dropdown
    std::vector<ProductSuggestion> currentSuggestions; // Results display
    std::string feedbackMessage = ""; // To show feedback after actions

    // Prepare category list for dropdown, adding "All Categories" at the beginning
    std::vector<std::string> categoriesForUI;
    categoriesForUI.push_back("All Categories"); // Add the special 'All' option
    std::vector<std::string> actualCategories = catalog.getCategories(); // Get from catalog
    categoriesForUI.insert(categoriesForUI.end(), actualCategories.begin(), actualCategories.end());

    // Convert C++ strings to const char* for ImGui Combo - must be done carefully!
    std::vector<const char*> categoryItems;
    categoryItems.reserve(categoriesForUI.size());
    for (const auto& cat : categoriesForUI) {
        categoryItems.push_back(cat.c_str());
    }

    // --- 7. Main Render Loop ---
    while (!glfwWindowShouldClose(window)) {
        // --- Event Handling ---
        glfwPollEvents(); // Poll inputs

        // --- Start ImGui Frame ---
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // --- Draw Your UI ---
        {
            ImGui::Begin("Product Search"); // Create a window

            // --- Category Selector ---
            if (ImGui::Combo("Category", &selectedCategoryIndex, categoryItems.data(), categoryItems.size())) {
                // Category changed, clear suggestions and feedback
                currentSuggestions.clear();
                feedbackMessage = "";
            }

            // Get the actual category name (or empty for "All")
            std::string currentCategory = "";
            if (selectedCategoryIndex > 0 && selectedCategoryIndex < categoriesForUI.size()) {
                currentCategory = categoriesForUI[selectedCategoryIndex];
            } // Index 0 means "All", so category remains ""

            ImGui::Separator();

            // --- Search Input ---
            if (ImGui::InputText("Search", searchQuery, sizeof(searchQuery), ImGuiInputTextFlags_EnterReturnsTrue)) {
                // User pressed Enter or focus lost - trigger search explicitly (or trigger on change)
                // No action needed here if we search every frame the text changes (below)
            }

            // Dynamic search: perform search whenever query or category changes
            // (Could add debounce for very large catalogs)
            currentSuggestions = catalog.searchProducts(searchQuery, currentCategory, 10); // Limit to 10 suggestions

            ImGui::Separator();

            // --- Display Suggestions ---
            ImGui::Text("Suggestions:");
            if (currentSuggestions.empty()) {
                ImGui::Text("  No products found matching '%s'", searchQuery);
            } else {
                 ImGui::BeginChild("SuggestionsScroll", ImVec2(0, ImGui::GetContentRegionAvail().y - 60), false, ImGuiWindowFlags_HorizontalScrollbar);
                 for (const auto& sugg : currentSuggestions) {
                     // Use Selectable to allow interaction
                     char buf[512];
                     snprintf(buf, sizeof(buf), "%-25s (ID: %d) - Pop: %.2f",
                              sugg.name.c_str(), sugg.productID, sugg.popularity);

                     if (ImGui::Selectable(buf)) {
                         // --- Action: Increase Popularity on Click ---
                         // Need the category the product *actually* belongs to!
                         // This requires reverse lookup or storing category with suggestion
                         // SIMPLIFICATION: Assume we know the category for now, OR only allow click-update when a category is selected
                         // Let's only allow boost if a specific category IS selected for simplicity here.
                         if (!currentCategory.empty()) {
                             if (catalog.updateProductPopularity(currentCategory, sugg.name, 2.0)) { // Boost by 2.0
                                 feedbackMessage = "Boosted popularity for: " + sugg.name;
                                 // Re-search immediately to show updated score (might reorder list)
                                 currentSuggestions = catalog.searchProducts(searchQuery, currentCategory, 10);
                             } else {
                                 feedbackMessage = "Error boosting popularity for: " + sugg.name;
                              }
                         } else {
                              feedbackMessage = "Popularity boost click only enabled when a specific category is selected.";
                         }

                     }
                 }
                 ImGui::EndChild();
            }

             ImGui::Separator();

            // --- Other Actions ---
             if (ImGui::Button("Apply 10% Decay")) {
                 catalog.applyGlobalDecay(0.90);
                 feedbackMessage = "Applied 10% popularity decay globally.";
                 // Re-search immediately to show updated scores
                 currentSuggestions = catalog.searchProducts(searchQuery, currentCategory, 10);
             }

             // --- Feedback Message Area ---
             if (!feedbackMessage.empty()) {
                 ImGui::TextWrapped("Info: %s", feedbackMessage.c_str());
             }

            ImGui::End(); // End the window
        }


        // --- Rendering ---
        ImGui::Render(); // Prepare draw data
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h); // Get window size
        glViewport(0, 0, display_w, display_h); // Set viewport
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f); // Set clear color (background)
        glClear(GL_COLOR_BUFFER_BIT); // Clear the screen

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); // Render ImGui data

        // --- Swap Buffers ---
        glfwSwapBuffers(window); // Display the rendered frame
    }

    // --- 8. Cleanup ---
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}