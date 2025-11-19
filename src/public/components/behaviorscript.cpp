#include <engine/public/components/behaviorscript.h>

BehaviorScript::BehaviorScript(std::unique_ptr<Behavior> behavior)
    : behavior_(std::move(behavior))
{
    Component::on_attach();

    if (!behavior_) {
        throw std::runtime_error("BehaviorScript has no associated Behavior.");
    }

    behavior_->attach(*this);
    behavior_->enable();
    behavior_->on_awake();
}

BehaviorScript::~BehaviorScript()
{
     if (!behavior_) {
        return;
    }

    behavior_->disable();
    behavior_->on_destroy();
}

void BehaviorScript::update(float dt)
{
    if (!enabled()) {
        behavior_->on_start();
        started_ = true;
        return;
    }

    behavior_->on_update(dt);
}

void BehaviorScript::on_serialize()
{
    // TODO: implement after networking is done
}

void BehaviorScript::on_deserialize()
{
    // TODO: implement after networking is done
}

Behavior& BehaviorScript::behavior()
{
    return *behavior_;
}

bool BehaviorScript::enabled() const
{
    return behavior_->enabled();
}

BehaviorScript& BehaviorScript::enable()
{
    behavior_->enable();
    return *this;
}

BehaviorScript& BehaviorScript::disable()
{
    behavior_->disable();
    return *this;
}