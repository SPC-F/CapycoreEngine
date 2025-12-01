#include <engine/core/engine.h>
#include <engine/core/system/system_service.h>
#include <engine/public/components/ui/image.h>
#include <engine/public/components/ui/text.h>
#include <engine/public/ui/ui_fps.h>
#include <engine/public/util/color.h>

constexpr char default_font[] = "ByteBounce";
constexpr char default_font_path[] =
    "resources/fonts/bytebounce/ByteBounce.ttf";
constexpr int default_font_size = 18;
constexpr int default_width = 80;
constexpr int default_height = 20;

UIFPS::UIFPS(Scene& scene)
    : UIObject(scene, default_width, default_height, Point{0.0f, 0.0f},
               Point{0.0f, 0.0f}) {
  int flip_x = 0;
  int flip_y = 0;
  add_component<Image>("", flip_x, flip_y, default_width, default_height,
                       Color{0, 0, 0, 255});
  add_component<Text>("fps: 0", default_font, default_font_path,
                      default_font_size, Color{0, 255, 0, 255})
      .alignment(TextAlignment::Left)
      .offset(Point{2.0f, 0.0f});
}

void UIFPS::update(float dt) {
  auto& system_service =
      Engine::instance().services->get_service<SystemService>().get();
  float fps_value = system_service.frames_per_second();

  if (auto text_component_opt = get_component<Text>();
      text_component_opt.has_value()) {
    auto& text_component = text_component_opt->get();
    text_component.text("fps: " + std::to_string(static_cast<int>(fps_value)));
  }
}