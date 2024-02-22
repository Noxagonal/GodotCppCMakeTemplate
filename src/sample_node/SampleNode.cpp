
#include "SampleNode.hpp"
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/godot.hpp>
#include <godot_cpp/variant/utility_functions.hpp>



void godot::SampleNode::_bind_methods()
{
	// Register your class methods and properties here.
	ClassDB::bind_method(D_METHOD("set_sample_property", "new_value"), &SampleNode::set_sample_property);
	ClassDB::bind_method(D_METHOD("get_sample_property"), &SampleNode::get_sample_property);
	ClassDB::add_property("SampleNode", PropertyInfo(Variant::Type::FLOAT, "sample_property"), "set_sample_property", "get_sample_property");

	ClassDB::bind_method(D_METHOD("sample_method"), &SampleNode::sample_method);

	ClassDB::bind_method(D_METHOD( "sample_method_with_parameters", "value" ), &SampleNode::sample_method_with_parameters);
}

godot::SampleNode::SampleNode()
{
	// Your class object initialization code here.
}

godot::SampleNode::~SampleNode()
{
	// Your class object unitialization code here.
}

void godot::SampleNode::_ready()
{
	// You should call base class _ready() to make sure that it gets processed. You should do this to all overridden functions.
	Node3D::_ready();
}

void godot::SampleNode::_process(double delta_time)
{
	// You should call base class _process() to make sure that it gets processed. You should do this to all overridden functions.
	Node3D::_process(delta_time);

	// This function is called in editor too, in this case we don't want to proceed.
	if (Engine::get_singleton()->is_editor_hint()) return;

	sample_property += delta_time;
	rotate(Vector3(0.0, 1.0, 0.0), 5.0 * delta_time);
}

double godot::SampleNode::get_sample_property()
{
	// Getter for property, it must return the value of property.
	return sample_property;
}

void godot::SampleNode::set_sample_property(double p_new_value)
{
	// Setter for property, it must assign the given argument to property.
	sample_property = p_new_value;
}

double godot::SampleNode::sample_method()
{
	return 50.05;
}

void godot::SampleNode::sample_method_with_parameters(uint64_t p_value)
{
	godot::UtilityFunctions::print("Called sample_method_with_parameters with argument: ", p_value);
}
