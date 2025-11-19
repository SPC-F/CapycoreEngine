#include <catch2/catch_test_macros.hpp>

#include <engine/core/behavior.h>
#include <engine/public/scene.h>
#include <engine/public/component.h>
#include <engine/public/components/behaviorscript.h>

TEST_CASE("behavior_runs_on_awake", "[Behavior]")
{
    class TestBehavior : public Behavior {
    public:
        void on_awake() override {
            awake_called = true;
        }
        void on_start() override {}
        void on_update(float dt) override {}
        void on_destroy() override {}

        bool awake_called = false;
    };

    Scene scene;
    GameObject game_object(scene);
    auto& behavior_script = game_object.add_component<BehaviorScript>(std::make_unique<TestBehavior>());

    REQUIRE(behavior_script.enabled() == true);
    REQUIRE(dynamic_cast<TestBehavior*>(&behavior_script.behavior())->awake_called == true);
}

TEST_CASE("behavior_on_start_and_update", "[Behavior]")
{
    class TestBehavior : public Behavior {
    public:
        void on_awake() override {}
        void on_start() override {
            start_called = true;
        }
        void on_update(float dt) override {
            update_called = true;
        }
        void on_destroy() override {}

        bool start_called = false;
        bool update_called = false;
    };

    Scene scene;
    GameObject game_object(scene);
    auto& behavior_script = game_object.add_component<BehaviorScript>(std::make_unique<TestBehavior>());

    constexpr float dt = 0.016f;

    behavior_script.update(dt);
    REQUIRE(dynamic_cast<TestBehavior*>(&behavior_script.behavior())->start_called == true);
    REQUIRE(dynamic_cast<TestBehavior*>(&behavior_script.behavior())->update_called == false);

    behavior_script.update(dt);
    REQUIRE(dynamic_cast<TestBehavior*>(&behavior_script.behavior())->update_called == true);
}

TEST_CASE("behavior_on_destroy_called_on_destruction", "[Behavior]")
{
    bool destroy_called = false;
    auto callback = [&destroy_called](Behavior& behavior) {
        destroy_called = true;
    };

    class TestBehavior : public Behavior {
    public:
        using callback_t = std::function<void(Behavior&)>;
        callback_t on_destroy_callback_;

        TestBehavior(std::function<void(Behavior&)> on_destroy_callback)
            : on_destroy_callback_(on_destroy_callback) {}

        void on_awake() override {}
        void on_start() override {}
        void on_update(float dt) override {}
        void on_destroy() override {
            on_destroy_callback_(*this);
        }
    };

    {
        Scene scene;
        GameObject game_object(scene);
        auto& behavior_script = game_object.add_component<BehaviorScript>(
            std::make_unique<TestBehavior>(callback)
        );

        REQUIRE(behavior_script.enabled() == true);
        game_object.remove_component(behavior_script);
    }

    REQUIRE(destroy_called == true);
}

TEST_CASE("behavior_can_access_game_object_and_transform", "[Behavior]")
{
    class TestBehavior : public Behavior {
    public:
        void on_awake() override {}
        void on_start() override {}
        void on_update(float dt) override {}
        void on_destroy() override {}
    };

    auto behavior = std::make_unique<TestBehavior>();

    Scene scene;
    GameObject game_object(scene);
    auto& behavior_script = game_object.add_component<BehaviorScript>(std::make_unique<TestBehavior>());
    
    REQUIRE(dynamic_cast<TestBehavior*>(&behavior_script.behavior())->game_object().id() == game_object.id());
    REQUIRE(&dynamic_cast<TestBehavior*>(&behavior_script.behavior())->transform() == &game_object.transform());
}