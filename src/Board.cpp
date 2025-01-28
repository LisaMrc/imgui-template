#include "../include/Board.hpp"
#include <imgui.h>
#include <iostream>
#include <vector>




void draw_board()
{
    // for (int i = 1; i <= 64; i++)
    // {
    //     if (i % 2 == 0)
    //     {
    //         ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.f, 0.f, 0.f, 1.f});
    //     }
    //     else
    //     {
    //         ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{1.f, 1.f, 1.f, 1.f});
    //     }
    //     ImGui::PushID(i); // When some ImGui items have the same label (for exemple the next two buttons are labeled "Yo") ImGui needs you to specify an ID so that it can distinguish them. It can be an int, a pointer, a string, etc.
    //     // You will definitely run into this when you create a button for each of your chess pieces, so remember to give them an ID!
    //     if (ImGui::Button("", ImVec2{100.f, 100.f}))
    //         std::cout << "Clicked button\n";
    //     if (i % 8 != 0)
    //         ImGui::SameLine();
    //     ImGui::PopID(); // Then pop the id you pushed after you created the widget
    //     ImGui::PopStyleColor();
    // }

    const int   rows       = 8;
    const int   cols       = 8;
    const float buttonSize = 100.0f;                          // Size of each button
    const float spacingX   = ImGui::GetStyle().ItemSpacing.x; // Horizontal spacing between buttons
    const float spacingY   = ImGui::GetStyle().ItemSpacing.y; // Vertical spacing between buttons

    // Calculate grid dimensions
    const float gridWidth  = (buttonSize + spacingX) * cols - spacingX;
    const float gridHeight = (buttonSize + spacingY) * rows - spacingY;

    // Calculate available space in the window
    float windowWidth  = ImGui::GetContentRegionAvail().x;
    float windowHeight = ImGui::GetContentRegionAvail().y;

    // Calculate offsets for centering
    float offsetX = (windowWidth - gridWidth) / 2.0f;
    float offsetY = (windowHeight - gridHeight) / 2.0f;

    // Adjust cursor position to center the grid
    if (offsetX > 0.0f)
    {
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offsetX);
    }
    if (offsetY > 0.0f)
    {
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + offsetY);
    }

    // Draw the grid
    for (int row = 0; row < rows; ++row)
    {
        for (int col = 0; col < cols; ++col)
        {
            // Generate the chessboard label (a1, b1, ..., h8)
            char label[4];
            snprintf(label, sizeof(label), "%c%d", 'a' + col, 8 - row);

            // Determine the square color (light or dark)
            bool   isDarkSquare = (row + col) % 2 == 1;
            ImVec4 normalColor  = isDarkSquare ? ImVec4(0.4f, 0.2f, 0.1f, 1.0f)  // Dark brown
                                               : ImVec4(0.9f, 0.9f, 0.9f, 1.0f); // Light beige

            // Define hover and active colors
            ImVec4 hoverColor  = isDarkSquare ? ImVec4(0.6f, 0.4f, 0.2f, 1.0f)  // Lighter dark
                                              : ImVec4(1.0f, 1.0f, 0.8f, 1.0f); // Lighter beige
            ImVec4 activeColor = isDarkSquare ? ImVec4(0.5f, 0.3f, 0.15f, 1.0f) // Darker dark
                                              : ImVec4(0.8f, 0.8f, 0.6f, 1.0f); // Darker beige

            // Push the button styles
            ImGui::PushStyleColor(ImGuiCol_Button, normalColor);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, hoverColor);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, activeColor);

            // Push the text color (black)
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));

            // Draw the button
            if (ImGui::Button(label, ImVec2(buttonSize, buttonSize)))
            {
                // Action when button is clicked
                printf("Button %s clicked\n", label);
            }

            // Pop the style colors
            ImGui::PopStyleColor(4); // 3 for button colors + 1 for text color

            // Place buttons on the same line, except for the last one in the row
            if (col < cols - 1)
            {
                ImGui::SameLine();
            }
        }
        // Adjust the cursor for centering the next row
        if (row < rows - 1 && offsetX > 0.0f)
        {
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offsetX);
        }
    }
}