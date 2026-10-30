//
// Created by dotryen on 10/27/26.
//

#if ME_IMGUI

#include <string>
#include <imgui.h>

#include "Texture.h"
#include "MECore/Math/Types.h"
#include "MECore/Script/Bindings.h"

namespace ME {
    static void RegisterWindowFlags(asIScriptEngine* engine) {
        int r;

        r = engine->RegisterEnum("ImGuiWindowFlags");
        assert(r >= 0);
        r = engine->RegisterEnumValue("ImGuiWindowFlags", "ImGuiWindowFlags_None", ImGuiWindowFlags_None);
        assert(r >= 0);
        r = engine->RegisterEnumValue("ImGuiWindowFlags", "ImGuiWindowFlags_NoTitleBar", ImGuiWindowFlags_NoTitleBar);
        assert(r >= 0);
        r = engine->RegisterEnumValue("ImGuiWindowFlags", "ImGuiWindowFlags_NoResize", ImGuiWindowFlags_NoResize);
        assert(r >= 0);
        r = engine->RegisterEnumValue("ImGuiWindowFlags", "ImGuiWindowFlags_NoMove", ImGuiWindowFlags_NoMove);
        assert(r >= 0);
        r = engine->RegisterEnumValue("ImGuiWindowFlags", "ImGuiWindowFlags_NoScrollbar", ImGuiWindowFlags_NoScrollbar);
        assert(r >= 0);
        r = engine->RegisterEnumValue("ImGuiWindowFlags", "ImGuiWindowFlags_NoScrollWithMouse", ImGuiWindowFlags_NoScrollWithMouse);
        assert(r >= 0);
        r = engine->RegisterEnumValue("ImGuiWindowFlags", "ImGuiWindowFlags_NoCollapse", ImGuiWindowFlags_NoCollapse);
        assert(r >= 0);
        r = engine->RegisterEnumValue("ImGuiWindowFlags", "ImGuiWindowFlags_AlwaysAutoResize", ImGuiWindowFlags_AlwaysAutoResize);
        assert(r >= 0);
        r = engine->RegisterEnumValue("ImGuiWindowFlags", "ImGuiWindowFlags_NoBackground", ImGuiWindowFlags_NoBackground);
        assert(r >= 0);
        r = engine->RegisterEnumValue("ImGuiWindowFlags", "ImGuiWindowFlags_NoSavedSettings", ImGuiWindowFlags_NoSavedSettings);
        assert(r >= 0);
        r = engine->RegisterEnumValue("ImGuiWindowFlags", "ImGuiWindowFlags_NoMouseInputs", ImGuiWindowFlags_NoMouseInputs);
        assert(r >= 0);
        r = engine->RegisterEnumValue("ImGuiWindowFlags", "ImGuiWindowFlags_MenuBar", ImGuiWindowFlags_MenuBar);
        assert(r >= 0);
        r = engine->RegisterEnumValue("ImGuiWindowFlags", "ImGuiWindowFlags_HorizontalScrollbar", ImGuiWindowFlags_HorizontalScrollbar);
        assert(r >= 0);
        r = engine->RegisterEnumValue("ImGuiWindowFlags", "ImGuiWindowFlags_NoFocusOnAppearing", ImGuiWindowFlags_NoFocusOnAppearing);
        assert(r >= 0);
        r = engine->RegisterEnumValue("ImGuiWindowFlags", "ImGuiWindowFlags_NoBringToFrontOnFocus", ImGuiWindowFlags_NoBringToFrontOnFocus);
        assert(r >= 0);
        r = engine->RegisterEnumValue("ImGuiWindowFlags", "ImGuiWindowFlags_AlwaysVerticalScrollbar", ImGuiWindowFlags_AlwaysVerticalScrollbar);
        assert(r >= 0);
        r = engine->RegisterEnumValue("ImGuiWindowFlags", "ImGuiWindowFlags_AlwaysHorizontalScrollbar", ImGuiWindowFlags_AlwaysHorizontalScrollbar);
        assert(r >= 0);
        r = engine->RegisterEnumValue("ImGuiWindowFlags", "ImGuiWindowFlags_NoNavInputs", ImGuiWindowFlags_NoNavInputs);
        assert(r >= 0);
        r = engine->RegisterEnumValue("ImGuiWindowFlags", "ImGuiWindowFlags_NoNavFocus", ImGuiWindowFlags_NoNavFocus);
        assert(r >= 0);
        r = engine->RegisterEnumValue("ImGuiWindowFlags", "ImGuiWindowFlags_UnsavedDocument", ImGuiWindowFlags_UnsavedDocument);
        assert(r >= 0);
        r = engine->RegisterEnumValue("ImGuiWindowFlags", "ImGuiWindowFlags_NoNav", ImGuiWindowFlags_NoNav);
        assert(r >= 0);
        r = engine->RegisterEnumValue("ImGuiWindowFlags", "ImGuiWindowFlags_NoDecoration", ImGuiWindowFlags_NoDecoration);
        assert(r >= 0);
        r = engine->RegisterEnumValue("ImGuiWindowFlags", "ImGuiWindowFlags_NoInputs", ImGuiWindowFlags_NoInputs);
        assert(r >= 0);
    }

    static void RegisterConditions(asIScriptEngine* engine) {
        int r;

        r = engine->RegisterEnum("ImGuiCond");
        assert(r >= 0);
        r = engine->RegisterEnumValue("ImGuiCond", "ImGuiCond_Always", ImGuiCond_Always);
        assert(r >= 0);
        r = engine->RegisterEnumValue("ImGuiCond", "ImGuiCond_Once", ImGuiCond_Once);
        assert(r >= 0);
        r = engine->RegisterEnumValue("ImGuiCond", "ImGuiCond_FirstUseEver", ImGuiCond_FirstUseEver);
        assert(r >= 0);
        r = engine->RegisterEnumValue("ImGuiCond", "ImGuiCond_Appearing", ImGuiCond_Appearing);
        assert(r >= 0);
    }

    static void RegisterTreeFlags(asIScriptEngine* engine) {
        int r;

        r = engine->RegisterEnum("ImGuiTreeNodeFlags");
        assert(r >= 0);
        r = engine->RegisterEnumValue("ImGuiTreeNodeFlags", "ImGuiTreeNodeFlags_None", ImGuiTreeNodeFlags_None);
        assert(r >= 0);
        r = engine->RegisterEnumValue("ImGuiTreeNodeFlags", "ImGuiTreeNodeFlags_Selected", ImGuiTreeNodeFlags_Selected);
        assert(r >= 0);
        r = engine->RegisterEnumValue("ImGuiTreeNodeFlags", "ImGuiTreeNodeFlags_Framed", ImGuiTreeNodeFlags_Framed);
        assert(r >= 0);
        r = engine->RegisterEnumValue("ImGuiTreeNodeFlags", "ImGuiTreeNodeFlags_AllowOverlap", ImGuiTreeNodeFlags_AllowOverlap);
        assert(r >= 0);
        r = engine->RegisterEnumValue("ImGuiTreeNodeFlags", "ImGuiTreeNodeFlags_NoTreePushOnOpen", ImGuiTreeNodeFlags_NoTreePushOnOpen);
        assert(r >= 0);
        r = engine->RegisterEnumValue("ImGuiTreeNodeFlags", "ImGuiTreeNodeFlags_NoAutoOpenOnLog", ImGuiTreeNodeFlags_NoAutoOpenOnLog);
        assert(r >= 0);
        r = engine->RegisterEnumValue("ImGuiTreeNodeFlags", "ImGuiTreeNodeFlags_DefaultOpen", ImGuiTreeNodeFlags_DefaultOpen);
        assert(r >= 0);
        r = engine->RegisterEnumValue("ImGuiTreeNodeFlags", "ImGuiTreeNodeFlags_OpenOnDoubleClick", ImGuiTreeNodeFlags_OpenOnDoubleClick);
        assert(r >= 0);
        r = engine->RegisterEnumValue("ImGuiTreeNodeFlags", "ImGuiTreeNodeFlags_OpenOnArrow", ImGuiTreeNodeFlags_OpenOnArrow);
        assert(r >= 0);
        r = engine->RegisterEnumValue("ImGuiTreeNodeFlags", "ImGuiTreeNodeFlags_Leaf", ImGuiTreeNodeFlags_Leaf);
        assert(r >= 0);
        r = engine->RegisterEnumValue("ImGuiTreeNodeFlags", "ImGuiTreeNodeFlags_Bullet", ImGuiTreeNodeFlags_Bullet);
        assert(r >= 0);
        r = engine->RegisterEnumValue("ImGuiTreeNodeFlags", "ImGuiTreeNodeFlags_FramePadding", ImGuiTreeNodeFlags_FramePadding);
        assert(r >= 0);
        r = engine->RegisterEnumValue("ImGuiTreeNodeFlags", "ImGuiTreeNodeFlags_SpanAvailWidth", ImGuiTreeNodeFlags_SpanAvailWidth);
        assert(r >= 0);
        r = engine->RegisterEnumValue("ImGuiTreeNodeFlags", "ImGuiTreeNodeFlags_SpanFullWidth", ImGuiTreeNodeFlags_SpanFullWidth);
        assert(r >= 0);
        r = engine->RegisterEnumValue("ImGuiTreeNodeFlags", "ImGuiTreeNodeFlags_SpanLabelWidth", ImGuiTreeNodeFlags_SpanLabelWidth);
        assert(r >= 0);
        r = engine->RegisterEnumValue("ImGuiTreeNodeFlags", "ImGuiTreeNodeFlags_SpanAllColumns", ImGuiTreeNodeFlags_SpanAllColumns);
        assert(r >= 0);
        r = engine->RegisterEnumValue("ImGuiTreeNodeFlags", "ImGuiTreeNodeFlags_LabelSpanAllColumns", ImGuiTreeNodeFlags_LabelSpanAllColumns);
        assert(r >= 0);
        r = engine->RegisterEnumValue("ImGuiTreeNodeFlags", "ImGuiTreeNodeFlags_NavLeftJumpsToParent", ImGuiTreeNodeFlags_NavLeftJumpsToParent);
        assert(r >= 0);
        r = engine->RegisterEnumValue("ImGuiTreeNodeFlags", "ImGuiTreeNodeFlags_CollapsingHeader", ImGuiTreeNodeFlags_CollapsingHeader);
        assert(r >= 0);
        r = engine->RegisterEnumValue("ImGuiTreeNodeFlags", "ImGuiTreeNodeFlags_DrawLinesNone", ImGuiTreeNodeFlags_DrawLinesNone);
        assert(r >= 0);
        r = engine->RegisterEnumValue("ImGuiTreeNodeFlags", "ImGuiTreeNodeFlags_DrawLinesFull", ImGuiTreeNodeFlags_DrawLinesFull);
        assert(r >= 0);
        r = engine->RegisterEnumValue("ImGuiTreeNodeFlags", "ImGuiTreeNodeFlags_DrawLinesToNodes", ImGuiTreeNodeFlags_DrawLinesToNodes);
        assert(r >= 0);
    }

    static void RegisterWindow(asIScriptEngine* engine) {
        int r;

        r = engine->RegisterGlobalFunction("bool Begin(const string&in name, int flags = 0)",
            asFUNCTIONPR([](const std::string& name, int flags) {
                return ImGui::Begin(name.c_str(), nullptr, flags);
            }, (const std::string&, int), bool),
            asCALL_CDECL);
        assert(r >= 0);
        // r = engine->RegisterGlobalFunction("bool Begin(const string&in, bool&inout, int=0)",
        //     asFUNCTIONPR([](const std::string& name, bool& opened, int flags) { return ImGui::Begin(name.c_str(), &opened, flags); }, (const std::string&, bool&, int), bool),
        //     asCALL_CDECL);
        assert(r >= 0);
        r = engine->RegisterGlobalFunction("void End()",
            asFUNCTION(ImGui::End),
            asCALL_CDECL);
        assert(r >= 0);

        r = engine->RegisterGlobalFunction("void SetNextWindowPos(const Vector2&in pos, int cond = 0, const Vector2&in pivot = Vector2(0.0, 0.0))",
            asFUNCTIONPR([](const Vector2& size, ImGuiCond cond, const Vector2& pivot) {
                ImGui::SetNextWindowPos(ImVec2(size[0], size[1]), cond, ImVec2(pivot[0], pivot[1]));
            }, (const Vector2&, ImGuiCond, const Vector2&), void),
            asCALL_CDECL);
        assert(r >= 0);
    }

    static void RegisterText(asIScriptEngine* engine) {
        int r;

        r = engine->RegisterGlobalFunction("void Text(const string&in text)",
            asFUNCTIONPR([](const std::string& str) {
                ImGui::Text(str.c_str());
            }, (const std::string&), void),
            asCALL_CDECL);
        assert(r >= 0);

        r = engine->RegisterGlobalFunction("void BulletText(const string&in text)",
            asFUNCTIONPR([](const std::string& str) {
                ImGui::BulletText(str.c_str());
            }, (const std::string&), void),
            asCALL_CDECL);
        assert(r >= 0);

        r = engine->RegisterGlobalFunction("void TextColored(const Vector4&in color, const string&in text)",
            asFUNCTIONPR([](const Vector4& col, const std::string& str) {
                ImGui::TextColored(ImVec4(col.GetX(), col.GetY(), col.GetZ(), col.GetW()), str.c_str());
            }, (const Vector4&, const std::string&), void),
            asCALL_CDECL);
        assert(r >= 0);
    }

    static void RegisterButtons(asIScriptEngine* engine) {
        int r;

        r = engine->RegisterGlobalFunction("bool Button(const string&in label)",
            asFUNCTIONPR([](const std::string& str) {
                return ImGui::Button(str.c_str());
            }, (const std::string&), bool),
            asCALL_CDECL);
        assert(r >= 0);

        r = engine->RegisterGlobalFunction("bool SmallButton(const string&in label)",
            asFUNCTIONPR([](const std::string& str) {
                return ImGui::SmallButton(str.c_str());
            }, (const std::string&), bool),
            asCALL_CDECL);
        assert(r >= 0);

        r = engine->RegisterGlobalFunction("bool Selectable(const string&in label, bool selected = false, int flags = 0, const Vector2&in size = Vector2(0, 0))",
            asFUNCTIONPR([](const std::string& str, bool selected, ImGuiSelectableFlags flags, const Vector2& size) {
                return ImGui::Selectable(str.c_str(), selected, flags, ImVec2(size[0], size[1]));
            }, (const std::string&, bool, int, const Vector2&), bool),
            asCALL_CDECL);
        assert(r >= 0);
    }

    static void RegisterTree(asIScriptEngine* engine) {
        int r;

        r = engine->RegisterGlobalFunction("bool TreeNode(string&in)",
            asFUNCTIONPR([](const std::string& str) {
                return ImGui::TreeNode(str.c_str());
            }, (const std::string&), bool),
            asCALL_CDECL);
        assert(r >= 0);
        r = engine->RegisterGlobalFunction("void TreePop()",
            asFUNCTION(ImGui::TreePop),
            asCALL_CDECL);
        assert(r >= 0);
    }

    static void RegisterListBox(asIScriptEngine* engine) {
        int r;

        r = engine->RegisterGlobalFunction("bool BeginListBox(const string&in label, const Vector2&in size = Vector2(0, 0))",
            asFUNCTIONPR([](const std::string& name, const Vector2& size) {
                return ImGui::BeginListBox(name.c_str(), ImVec2(size[0], size[1]));
            }, (const std::string&, const Vector2&), bool),
            asCALL_CDECL);
        assert(r >= 0);

        r = engine->RegisterGlobalFunction("void EndListBox()",
            asFUNCTION(ImGui::EndListBox),
            asCALL_CDECL);
        assert(r >= 0);
    }

    static void RegisterImage(asIScriptEngine* engine) {
        int r;

        r = engine->RegisterGlobalFunction("void Image(Texture&in texture, const Vector2&in size, const Vector2&in uv0 = Vector2(0, 0), const Vector2&in uv1 = Vector2(1, 1))",
                asFUNCTIONPR([](Diligent::ITexture*& texture, const Vector2& size, const Vector2& uv0, const Vector2& uv1) {
                    ImGui::Image(ImTextureRef(texture->GetDefaultView(Diligent::TEXTURE_VIEW_SHADER_RESOURCE)),
                        ImVec2(size[0], size[1]),
                        ImVec2(uv0[0], uv0[1]),
                        ImVec2(uv1[0], uv1[1]));
                }, (Diligent::ITexture*&, const Vector2&, const Vector2&, const Vector2&), void),
                asCALL_CDECL);
        assert(r >= 0);
    }

    void RegisterImGui(asIScriptEngine* engine) {
        int r;

        RegisterWindowFlags(engine);
        RegisterConditions(engine);
        RegisterTreeFlags(engine);

        r = engine->SetDefaultNamespace("ImGui");
        assert(r >= 0);

        RegisterWindow(engine);
        RegisterText(engine);
        RegisterButtons(engine);
        RegisterTree(engine);
        RegisterListBox(engine);
        RegisterImage(engine);

        r = engine->RegisterGlobalFunction("void ShowDemoWindow()",
            asFUNCTION(ImGui::ShowDemoWindow),
            asCALL_CDECL);
        assert(r >= 0);

        r = engine->RegisterGlobalFunction("void SameLine(float=0, float=-1)",
            asFUNCTION(ImGui::SameLine),
            asCALL_CDECL);
        assert(r >= 0);

        r = engine->SetDefaultNamespace("");
        assert(r >= 0);
    }
}

#endif
