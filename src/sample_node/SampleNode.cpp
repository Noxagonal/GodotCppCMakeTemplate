
#include "SampleNode.hpp"
#include <godot_cpp/classes/engine.hpp>



void godot::SampleNode::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("set_sample_property", "new_value"), &SampleNode::set_sample_property);
	ClassDB::bind_method(D_METHOD("get_sample_property"), &SampleNode::get_sample_property);
	ClassDB::add_property("SampleNode", PropertyInfo(Variant::Type::FLOAT, "sample_property"), "set_sample_property", "get_sample_property");

	ClassDB::bind_method(D_METHOD("sample_method"), &SampleNode::sample_method);
}

godot::SampleNode::SampleNode()
{
}

godot::SampleNode::~SampleNode()
{
}

void godot::SampleNode::_ready()
{
}

void godot::SampleNode::_process(double delta_time)
{
	if (Engine::get_singleton()->is_editor_hint()) return;

	sample_property += delta_time;
	rotate(Vector3(0.0, 1.0, 0.0), 5.0 * delta_time);
}

double godot::SampleNode::get_sample_property()
{
	return sample_property;
}

void godot::SampleNode::set_sample_property(double new_value)
{
	sample_property = new_value;
}

double godot::SampleNode::sample_method()
{
	return sample_property;
}
