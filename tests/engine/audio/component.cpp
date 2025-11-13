#include <catch2/catch_test_macros.hpp>

#include <engine/core/engine.h>
#include <engine/audio/audio_service.h>
#include <engine/public/components/audio_source.h>
#include <engine/public/scene.h>
#include <engine/public/gameObject.h>

TEST_CASE("audio_source_component_plays_and_stops_sounds_correctly", "[AudioSourceComponent]") {
    auto file_path = std::string(PROJECT_ROOT) + "/resources/sounds/bonetrousle.wav";

    auto& audio_service = Engine::instance().services->get_service<AudioService>().get();
    Scene scene;
    GameObject game_object {scene};

    game_object.add_component<AudioSource>(file_path, true, false, 0.7f);
    audio_service.update();

    SECTION("audio_source_plays_sound_correctly") {
        auto playing_sounds = audio_service.get_all_playing_sounds();
        REQUIRE(playing_sounds.size() == 1);
        REQUIRE(playing_sounds[0].get().resource()->file_path() == file_path);
        REQUIRE(playing_sounds[0].get().volume() == 0.7f);
    }

    SECTION("audio_source_stops_sound_correctly") {
        auto comp_opt = game_object.get_component<AudioSource>();
        REQUIRE(comp_opt.has_value());
        auto sound_name = comp_opt->get().audio_name();
        game_object.remove_component<AudioSource>(comp_opt->get());
        audio_service.update();

        auto playing_sounds = audio_service.get_all_playing_sounds();
        REQUIRE(comp_opt->get().instance() == std::nullopt);
    }
}

TEST_CASE("audio_source_component_modifies_properties_correctly", "[AudioSourceComponent]") {
    auto file_path = std::string(PROJECT_ROOT) + "/resources/sounds/bonetrousle.wav";

    Scene scene;
    GameObject game_object {scene};

    auto& audio_source = game_object.add_component<AudioSource>(file_path, false, false, 0.6f);

    REQUIRE(audio_source.audio_file_path() == file_path);
    REQUIRE(audio_source.volume() == 0.6f);
    REQUIRE_FALSE(audio_source.loop());
    REQUIRE_FALSE(audio_source.play_on_awake());

    SECTION("modifying_audio_source_properties_works") {
        audio_source.audio_file_path("./new/path/to/sound.wav");
        audio_source.volume(0.9f);
        audio_source.loop(true);
        audio_source.play_on_awake(true);

        REQUIRE(audio_source.audio_file_path() == "./new/path/to/sound.wav");
        REQUIRE(audio_source.volume() == 0.9f);
        REQUIRE(audio_source.loop());
        REQUIRE(audio_source.play_on_awake());
    }
}