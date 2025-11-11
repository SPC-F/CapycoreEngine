#include <map>

#include <engine/input/i_input_provider.h>
#include <engine/input/key_code.h>
#include <engine/input/key_state.h>

/**
 * @brief SDL-backed implementation of the input provider interface.
 *
 * Polls keyboard state from SDL each frame and tracks key transitions.
 * Acts as the bridge between SDL event data and the engine’s logical input model.
 */
class InputSystem : public IInputProvider {
public:
    bool is_key_held(KeyCode key) const override;
    bool is_key_pressed(KeyCode key) const override;
    bool is_key_released(KeyCode key) const override;
    bool any_key_held() const override;
    bool any_key_pressed() const override;
    bool any_key_released() const override;
    float key_hold_duration(KeyCode key) const override;
    std::optional<KeyCode> last_key_pressed() const override;

    bool is_mouse_held(MouseButton button) const override;
    bool is_mouse_pressed(MouseButton button) const override;
    bool is_mouse_released(MouseButton button) const override;
    bool any_mouse_held() const override;
    bool any_mouse_pressed() const override;
    bool any_mouse_released() const override;
    size_t mouse_hold_duration(MouseButton button) const override;
    std::optional<MouseButton> last_mouse_pressed() const override;

    const Point mouse_position() const override;
    const std::pair<float, float> mouse_delta() const override;
    bool mouse_moved() const override;
    const std::pair<float, float> mouse_scroll() const override;
    const std::pair<float, float> mouse_scroll_delta() const override;
    MouseDirection mouse_scroll_direction() const override;

    void reset_state() override;
    void update() override;

protected:
    /**
     * @brief Per-key state cache for frame-level input tracking.
     *
     * Indexed by logical key to decouple SDL scancodes from engine logic.
     * Internal only—consumers should query through the public interface.
     */
    std::map<KeyCode, KeyState> key_states_;

    /**
    * @brief Mouse state cache for frame-level input tracking.
    *
    * Tracks button states, cursor position, movement deltas, and scroll information.
    * Indexed by logical mouse button to decouple backend events from engine logic.
    * Internal only—consumers should query through the public interface.
    */
    MouseState mouse_state_;
};
