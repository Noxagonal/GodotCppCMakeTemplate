#pragma once

#include <godot_cpp/classes/node3d.hpp>



namespace godot {

class SampleNode : public Node3D
{
	GDCLASS(SampleNode, Node3D);

protected:
	// Every registered class must have this method. It is used to register methods and properties of this class.
	static void _bind_methods();

public:
	SampleNode();
	virtual ~SampleNode();

	// Called when a node is added to the scene tree.
	virtual void _ready() override;

	// Called once a frame.
	virtual void _process(double delta_time) override;

	// This is a simple property with its getter and setter. Every property will require its own
	// getter and setter in order for it to be visible to Godot, Eg. in GDScript.
	double sample_property = 0.0;
	double get_sample_property();
	void set_sample_property(double p_new_value);

	// Simple sample method returning something.
	double sample_method();

	// Simple method accepting an argument.
	void sample_method_with_parameters(uint64_t p_value);
};

}
