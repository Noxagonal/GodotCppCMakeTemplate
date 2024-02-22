#pragma once

#include <godot_cpp/classes/node3d.hpp>



namespace godot {

class SampleNode : public Node3D
{
	GDCLASS(SampleNode, Node3D);

protected:
	static void _bind_methods();

public:
	SampleNode();
	~SampleNode();

	void _ready() override;
	void _process(double delta_time) override;

	double sample_property = 0.0;
	double get_sample_property();
	void set_sample_property(double new_value);

	double sample_method();

};

}
