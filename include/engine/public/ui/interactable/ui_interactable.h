#pragma once

#include <functional>
#include <vector>

#include <engine/public/ui/ui_object.h>

/**
 * @brief Interaction state for UIInteractable elements.
 * 
 * Tracks hover, press, hold, focus, and disabled states.
 */
struct UIInteractionState {
    bool hovered   = false;
    bool pressed   = false;
    bool held      = false;
    bool focused   = false;
    bool disabled  = false;
};

/**
 * @brief Base class for interactive UI elements.
 * 
 * UIInteractable extends UIObject to provide interaction capabilities
 * such as hover, press, hold, and focus states. It includes event hooks
 * that can be overridden by derived classes or have handlers attached.
 */
class UIInteractable : public UIObject {
public:
    UIInteractable(
        Scene& scene,
        float width,
        float height,
        Point pivot,
        Point anchor
    );

    /**
     * @brief Update interaction state based on input.
     * 
     * The update_interaction method checks for mouse position and button states
     * to update the interaction state of the UI element accordingly.
     */
    void update_interaction(float dt);

    UIInteractionState& state();
    [[nodiscard]] const UIInteractionState& state() const;

    /** 
     * @brief Disable the interactable element.
     */
    void disable();
    /** 
     * @brief Enable the interactable element.
     */
    void enable();

    /**
     * @brief Event hook for when the element is hovered.
     */
    virtual void on_hover() {}

    /**
     * @brief Event hook for when the element is unhovered.
     */
    virtual void on_unhover() {}

    /**
     * @brief Event hook for when the element is pressed.
     */
    virtual void on_press() {}
    
    /**
     * @brief Event hook for when the element is released.
     */
    virtual void on_release() {}

    /**
     * @brief Event hook for when the element is held down.
     */
    virtual void on_hold() {}

    /**
     * @brief Event hook for when the element gains focus.
     */
    virtual void on_focus() {}

    /**
     * @brief Event hook for when the element loses focus.
     */
    virtual void on_unfocus() {}

    void add_on_hover(const std::function<void(UIInteractable&)>& handler);
    void add_on_unhover(const std::function<void(UIInteractable&)>& handler);

    void add_on_press(const std::function<void(UIInteractable&)>& handler);
    void add_on_release(const std::function<void(UIInteractable&)>& handler);

    void add_on_focus(const std::function<void(UIInteractable&)>& handler);
    void add_on_unfocus(const std::function<void(UIInteractable&)>& handler);

protected:
    [[nodiscard]] bool in_range(const Point& mouse_pos) const;

    UIInteractionState interaction_state_;

    std::vector<std::function<void(UIInteractable&)>> on_hover_handlers_;
    std::vector<std::function<void(UIInteractable&)>> on_unhover_handlers_;

    std::vector<std::function<void(UIInteractable&)>> on_press_handlers_;
    std::vector<std::function<void(UIInteractable&)>> on_release_handlers_;

    std::vector<std::function<void(UIInteractable&)>> on_focus_handlers_;
    std::vector<std::function<void(UIInteractable&)>> on_unfocus_handlers_;
};
