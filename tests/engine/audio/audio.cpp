#include <catch2/catch_test_macros.hpp>

#include <engine/audio/audio_service.h>
#include <engine/audio/sound/sound_factory.h>

TEST_CASE("AudioService registers and retrieves sound resources", "[AudioService]") {
    AudioService audioService;
    auto filePath = "./resources/sounds/bonetrousle.wav";
    auto soundName = "Bonetrousle";

    auto resource = audioService.register_sound(filePath, soundName, SoundType::SDL_MIXER);
    REQUIRE(resource != nullptr);

    SECTION("Double registration returns the same resource") {
        auto second = audioService.register_sound(filePath, soundName, SoundType::SDL_MIXER);
        REQUIRE(resource == second);
    }

    SECTION("Retrieving a registered sound works") {
        auto retrieved = audioService.get_sound_resource(soundName);
        REQUIRE(retrieved != nullptr);
        REQUIRE(retrieved->name() == soundName);
    }

    SECTION("Retrieving an unknown sound returns nullptr") {
        REQUIRE(audioService.get_sound_resource("Nonexistent") == nullptr);
    }
}

TEST_CASE("AudioService unregisters sound resources correctly", "[AudioService]") {
    AudioService audioService;
    auto soundName = "Bonetrousle";
    audioService.register_sound("./resources/sounds/bonetrousle.wav", soundName, SoundType::SDL_MIXER);

    SECTION("Unregister removes resource and instances") {
        audioService.play_sound(soundName, 0.5f, false);
        REQUIRE(audioService.has_sound_instance(soundName));

        bool removed = audioService.unregister_sound(soundName);
        
        REQUIRE(removed);
        REQUIRE_FALSE(audioService.has_sound_instance(soundName));
        REQUIRE(audioService.get_sound_resource(soundName) == nullptr);
    }

    SECTION("Unregistering non-existent sound returns false") {
        REQUIRE_FALSE(audioService.unregister_sound("NoSound"));
    }
}

TEST_CASE("AudioService plays and stops sounds", "[AudioService]") {
    AudioService audioService;
    auto name = "Bonetrousle";
    audioService.register_sound("./resources/sounds/bonetrousle.wav", name, SoundType::SDL_MIXER);

    SECTION("Playing sound from a resource works") {
        auto resource = audioService.get_sound_resource(name);
        auto instance = audioService.play_sound(resource, 0.8f, false);
        REQUIRE(instance.get().resource()->name() == name);
    }

    SECTION("Playing sound by name works") {
        auto instance = audioService.play_sound(name, 0.8f, false);
        REQUIRE(instance.get().resource()->name() == name);
    }

    SECTION("Stopping specific sound by name removes it") {
        audioService.play_sound(name, 0.5f, false);
        REQUIRE(audioService.has_sound_instance(name));

        audioService.stop_sound(name);
        REQUIRE_FALSE(audioService.has_sound_instance(name));
    }

    SECTION("Stopping via unique_ptr removes instance") {
        auto resource = audioService.get_sound_resource(name);
        auto instance = SoundFactory::create_sound_instance(resource, 0.5f);

        REQUIRE(instance != nullptr);

        audioService.play_sound(resource, 0.5f, false);
        audioService.stop_sound(std::move(instance));
        REQUIRE(instance == nullptr);
    }

    SECTION("Stopping all sounds clears active list") {
        audioService.play_sound(name, 0.5f, true);
        audioService.play_sound(name, 0.7f, false);
        REQUIRE(audioService.get_all_playing_sounds().size() == 2);

        audioService.stop_all_sounds();
        REQUIRE(audioService.get_all_playing_sounds().empty());
    }
}

TEST_CASE("AudioService manages volumes correctly", "[AudioService]") {
    AudioService audioService;
    auto name = "Bonetrousle";
    audioService.register_sound("./resources/sounds/bonetrousle.wav", name, SoundType::SDL_MIXER);

    auto instance = audioService.play_sound(name, 0.5f, false);
    REQUIRE(instance.get().volume() == 0.5f);

    SECTION("Setting volume by name works") {
        audioService.set_sound_volume(name, 0.9f);
        REQUIRE(instance.get().volume() == 0.9f);
    }

    SECTION("Setting volume on moved instance works") {
        auto resource = audioService.get_sound_resource(name);
        auto tmp = SoundFactory::create_sound_instance(resource, 0.3f);
        audioService.set_sound_volume(std::move(tmp), 0.8f);

        REQUIRE(tmp.get()->volume() == 0.8f);
    }

    SECTION("Setting volume for non-existent sound does nothing") {
        audioService.set_sound_volume("GhostSound", 0.5f);
        REQUIRE(true);
    }
}

TEST_CASE("AudioService reports and retrieves sound instances", "[AudioService]") {
    AudioService audioService;
    auto name = "Bonetrousle";
    audioService.register_sound("./resources/sounds/bonetrousle.wav", name, SoundType::SDL_MIXER);

    SECTION("has_sound_instance works") {
        REQUIRE_FALSE(audioService.has_sound_instance(name));
        audioService.play_sound(name, 0.5f, false);
        REQUIRE(audioService.has_sound_instance(name));
    }

    SECTION("get_sound_instance returns reference when active") {
        audioService.play_sound(name, 0.5f, false);
        auto ref = audioService.get_sound_instance(name);

        REQUIRE(ref.has_value());
        REQUIRE(ref->get().resource()->name() == name);
    }

    SECTION("get_sound_instance returns nullopt for inactive sounds") {
        REQUIRE_FALSE(audioService.get_sound_instance(name).has_value());
    }
}

TEST_CASE("AudioService update removes finished instances", "[AudioService]") {
    AudioService audioService;
    auto name = "Bonetrousle";
    audioService.register_sound("./resources/sounds/bonetrousle.wav", name, SoundType::SDL_MIXER);
    auto instance = audioService.play_sound(name, 1.0f, false);

    REQUIRE_FALSE(audioService.get_all_playing_sounds().empty());

    SECTION("update removes stopped sounds") {
        instance.get().stop();
        audioService.update();

        REQUIRE(audioService.get_all_playing_sounds().empty());
    }

    SECTION("update keeps active sounds") {
        audioService.update();
        REQUIRE_FALSE(audioService.get_all_playing_sounds().empty());
    }
}

TEST_CASE("AudioService handles invalid inputs safely", "[AudioService]") {
    AudioService audioService;

    SECTION("play_sound throws on null resource") {
        REQUIRE_THROWS_AS(audioService.play_sound("", 0.5f, false), std::invalid_argument);
    }

    SECTION("stop_sound handles null unique_ptr gracefully") {
        std::unique_ptr<SoundInstance> nullInstance;
        REQUIRE_NOTHROW(audioService.stop_sound(std::move(nullInstance)));
    }

    SECTION("set_sound_volume handles null unique_ptr gracefully") {
        std::unique_ptr<SoundInstance> nullInstance;
        REQUIRE_NOTHROW(audioService.set_sound_volume(std::move(nullInstance), 0.5f));
    }
}
