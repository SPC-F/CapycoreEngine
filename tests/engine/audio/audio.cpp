#include <catch2/catch_test_macros.hpp>

#include <engine/audio/audio_service.h>
#include <engine/audio/sound/sound_factory.h>

auto file_path = std::string(PROJECT_ROOT) + "/resources/sounds/bonetrousle.wav";

TEST_CASE("audio_service_register_should_return_resource_when_registering_valid_sound", "[AudioService]") {
    AudioService audio_service;
    auto sound_name = "Bonetrousle";

    auto resource = audio_service.register_sound(file_path, sound_name, SoundType::SDL_MIXER);
    REQUIRE(resource != nullptr);

    SECTION("registering_same_sound_name_returns_existing_resource") {
        auto second = audio_service.register_sound(file_path, sound_name, SoundType::SDL_MIXER);
        REQUIRE(resource == second);
    }

    SECTION("retrieving_registered_sound_returns_correct_resource") {
        auto retrieved = audio_service.get_sound_resource(sound_name);
        REQUIRE(retrieved != nullptr);
        REQUIRE(retrieved->name() == sound_name);
    }

    SECTION("retrieving_unregistered_sound_returns_nullptr") {
        REQUIRE(audio_service.get_sound_resource("Nonexistent") == nullptr);
    }
}

TEST_CASE("audio_service_unregisters_sound_resources_correctly", "[AudioService]") {
    AudioService audio_service;
    auto sound_name = "Bonetrousle";
    audio_service.register_sound("./resources/sounds/bonetrousle.wav", sound_name, SoundType::SDL_MIXER);

    SECTION("unregistering_existing_sound_removes_resource_and_instances") {
        audio_service.play_sound(sound_name, 0.5f, false);
        REQUIRE(audio_service.has_sound_instance(sound_name));

        bool removed = audio_service.unregister_sound(sound_name);

        REQUIRE(removed);
        REQUIRE_FALSE(audio_service.has_sound_instance(sound_name));
        REQUIRE(audio_service.get_sound_resource(sound_name) == nullptr);
    }

    SECTION("unregistering_non_existent_sound_returns_false") {
        REQUIRE_FALSE(audio_service.unregister_sound("NoSound"));
    }
}

TEST_CASE("audio_service_plays_and_stops_sounds", "[AudioService]") {
    AudioService audio_service;
    auto name = "Bonetrousle";
    audio_service.register_sound("./resources/sounds/bonetrousle.wav", name, SoundType::SDL_MIXER);

    SECTION("playing_sound_by_resource_works") {
        auto resource = audio_service.get_sound_resource(name);
        auto instance = audio_service.play_sound(resource, 0.8f, false);
        REQUIRE(instance.get().resource()->name() == name);
    }

    SECTION("playing_sound_by_name_works") {
        auto instance = audio_service.play_sound(name, 0.8f, false);
        REQUIRE(instance.get().resource()->name() == name);
    }

    SECTION("stopping_specific_sound_by_name_removes_it") {
        audio_service.play_sound(name, 0.5f, false);
        REQUIRE(audio_service.has_sound_instance(name));

        audio_service.stop_sound(name);
        REQUIRE_FALSE(audio_service.has_sound_instance(name));
    }

    SECTION("stopping_via_unique_ptr_removes_instance") {
        auto resource = audio_service.get_sound_resource(name);
        auto instance = SoundFactory::create_sound_instance(resource, 0.5f);

        REQUIRE(instance != nullptr);

        audio_service.play_sound(resource, 0.5f, false);
        audio_service.stop_sound(std::move(instance));
        REQUIRE(instance == nullptr);
    }

    SECTION("stopping_all_sounds_clears_active_list") {
        audio_service.play_sound(name, 0.5f, true);
        audio_service.play_sound(name, 0.7f, false);
        REQUIRE(audio_service.get_all_playing_sounds().size() == 2);

        audio_service.stop_all_sounds();
        REQUIRE(audio_service.get_all_playing_sounds().empty());
    }
}

TEST_CASE("audio_service_manages_volumes_correctly", "[AudioService]") {
    AudioService audio_service;
    auto name = "Bonetrousle";
    audio_service.register_sound("./resources/sounds/bonetrousle.wav", name, SoundType::SDL_MIXER);

    auto instance = audio_service.play_sound(name, 0.5f, false);
    REQUIRE(instance.get().volume() == 0.5f);

    SECTION("setting_volume_by_name_works") {
        instance.get().volume(0.9f);
        REQUIRE(instance.get().volume() == 0.9f);
    }

    SECTION("setting_volume_on_moved_instance_works") {
        auto resource = audio_service.get_sound_resource(name);
        auto tmp = SoundFactory::create_sound_instance(resource, 0.3f);
        tmp.get()->volume(0.8f);

        REQUIRE(tmp.get()->volume() == 0.8f);
    }
}

TEST_CASE("audio_service_reports_and_retrieves_sound_instances", "[AudioService]") {
    AudioService audio_service;
    auto name = "Bonetrousle";
    audio_service.register_sound("./resources/sounds/bonetrousle.wav", name, SoundType::SDL_MIXER);

    SECTION("has_sound_instance works") {
        REQUIRE_FALSE(audio_service.has_sound_instance(name));
        audio_service.play_sound(name, 0.5f, false);
        REQUIRE(audio_service.has_sound_instance(name));
    }

    SECTION("get_sound_instance returns reference when active") {
        audio_service.play_sound(name, 0.5f, false);
        auto ref = audio_service.get_sound_instance(name);

        REQUIRE(ref.has_value());
        REQUIRE(ref->get().resource()->name() == name);
    }

    SECTION("get_sound_instance returns nullopt for inactive sounds") {
        REQUIRE_FALSE(audio_service.get_sound_instance(name).has_value());
    }
}

TEST_CASE("audio_service_updates_sound_instances_correctly", "[AudioService]") {
    AudioService audio_service;
    auto name = "Bonetrousle";
    audio_service.register_sound("./resources/sounds/bonetrousle.wav", name, SoundType::SDL_MIXER);
    auto instance = audio_service.play_sound(name, 1.0f, false);

    REQUIRE_FALSE(audio_service.get_all_playing_sounds().empty());

    SECTION("update_removes_finished_sounds") {
        instance.get().stop();
        audio_service.update();

        REQUIRE(audio_service.get_all_playing_sounds().empty());
    }

    SECTION("update_keeps_playing_sounds") {
        audio_service.update();
        REQUIRE_FALSE(audio_service.get_all_playing_sounds().empty());
    }
}

TEST_CASE("audio_service_handles_invalid_inputs_safely", "[AudioService]") {
    AudioService audio_service;

    SECTION("play_sound_throws_on_null_resource") {
        REQUIRE_THROWS_AS(audio_service.play_sound("", 0.5f, false), std::invalid_argument);
    }

    SECTION("stop_sound_handles_null_unique_ptr_gracefully") {
        std::unique_ptr<SoundInstance> nullInstance;
        REQUIRE_NOTHROW(audio_service.stop_sound(std::move(nullInstance)));
    }
}
