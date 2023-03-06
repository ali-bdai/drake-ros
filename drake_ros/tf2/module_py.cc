#include <memory>

#include <drake/systems/framework/diagram.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "drake_ros/core/drake_ros.h"
#include "drake_ros/tf2/drake_ros_tf2_pybind.h"
#include "drake_ros/tf2/scene_tf_broadcaster_system.h"

namespace drake_ros_tf2 {
namespace drake_ros_tf2_py {
namespace {

using drake::systems::Diagram;
using drake::systems::TriggerType;
using drake_ros_core::DrakeRos;

PYBIND11_MODULE(drake_ros_tf2, m) {
  m.doc() = "Python wrapper for drake_ros_tf2";

  py::module::import("drake_ros_core");

  py::module::import("pydrake.systems.framework");
  py::module::import("pydrake.multibody.plant");

  const SceneTfBroadcasterParams default_params{};
  py::class_<SceneTfBroadcasterParams>(m, "SceneTfBroadcasterParams")
      .def(
          py::init([](const std::unordered_set<drake::systems::TriggerType>&
                          publish_triggers,
                      double publish_period, const std::string& tf_topic_name) {
            return SceneTfBroadcasterParams{publish_triggers, publish_period,
                                            tf_topic_name};
          }),
          py::kw_only(),
          py::arg("publish_triggers") = default_params.publish_triggers,
          py::arg("publish_period") = default_params.publish_period,
          py::arg("tf_topic_name") = default_params.tf_topic_name)
      .def_readwrite("publish_triggers",
                     &SceneTfBroadcasterParams::publish_triggers)
      .def_readwrite("publish_period",
                     &SceneTfBroadcasterParams::publish_period)
      .def_readwrite("tf_topic_name", &SceneTfBroadcasterParams::tf_topic_name);

  py::class_<SceneTfBroadcasterSystem, Diagram<double>>(
      m, "SceneTfBroadcasterSystem")
      .def(py::init<DrakeRos*, SceneTfBroadcasterParams>(), py::arg("ros"),
           py::arg("params") = SceneTfBroadcasterParams{})
      .def("RegisterMultibodyPlant",
           &SceneTfBroadcasterSystem::RegisterMultibodyPlant)
      .def("ComputeFrameHierarchy",
           &SceneTfBroadcasterSystem::ComputeFrameHierarchy)
      .def("get_graph_query_input_port",
           &SceneTfBroadcasterSystem::get_graph_query_input_port,
           py::return_value_policy::reference_internal);
}

}  // namespace
}  // namespace drake_ros_tf2_py
}  // namespace drake_ros_tf2
