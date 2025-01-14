#pragma once

#include "../include/Board.hpp"

#include <imgui.h>
#include <iostream>

void draw_board() {
  float value{0.f};
  ImGui::SliderFloat("My Value", &value, 0.f, 3.f);

  if (ImGui::Button("1", ImVec2{50.f, 50.f}))
    std::cout << "Clicked button 1\n";
  ImGui::SameLine(); // Draw the next ImGui widget on the same line as the
                     // previous one. Otherwise it would be below it

  ImGui::PushStyleColor(
      ImGuiCol_Button,
      ImVec4{1.f, 0.f, 0.f,
             1.f}); // Changes the color of all buttons until we call
                    // ImGui::PopStyleColor(). There is also
                    // ImGuiCol_ButtonActive and ImGuiCol_ButtonHovered

  ImGui::PushID(
      2); // When some ImGui items have the same label (for exemple the next two
          // buttons are labeled "Yo") ImGui needs you to specify an ID so that
          // it can distinguish them. It can be an int, a pointer, a string,
          // etc. You will definitely run into this when you create a button for
          // each of your chess pieces, so remember to give them an ID!
  if (ImGui::Button("Yo", ImVec2{50.f, 50.f}))
    std::cout << "Clicked button 2\n";
  ImGui::PopID(); // Then pop the id you pushed after you created the widget

  ImGui::SameLine();
  ImGui::PushID(3);
  if (ImGui::Button("Yo", ImVec2{50.f, 50.f}))
    std::cout << "Clicked button 3\n";
  ImGui::PopID();

  ImGui::PopStyleColor();
}