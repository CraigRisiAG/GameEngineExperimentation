

/**
 * @file spatial_editor_gizmos.h
 * @brief Header file containing various gizmo plugin classes for the spatial
 * editor.
 *
 * This file defines a collection of EditorSpatialGizmoPlugin-derived classes
 * that provide visual gizmo representations and interaction handles for
 * different 3D node types in the editor.
 *
 * @details The file includes gizmo plugins for:
 *   - Light sources (LightSpatialGizmoPlugin)
 *   - Audio nodes (AudioStreamPlayer3DSpatialGizmoPlugin)
 *   - Cameras (CameraSpatialGizmoPlugin)
 *   - Mesh instances (MeshInstanceSpatialGizmoPlugin)
 *   - Sprites (Sprite3DSpatialGizmoPlugin)
 *   - Position markers (Position3DSpatialGizmoPlugin)
 *   - Skeletons (SkeletonSpatialGizmoPlugin)
 *   - Physical bones (PhysicalBoneSpatialGizmoPlugin)
 *   - Ray casting nodes (RayCastSpatialGizmoPlugin)
 *   - Spring arms (SpringArmSpatialGizmoPlugin)
 *   - Vehicle wheels (VehicleWheelSpatialGizmoPlugin)
 *   - Soft bodies (SoftBodySpatialGizmoPlugin)
 *   - Visibility notifiers (VisibilityNotifierGizmoPlugin)
 *   - Particles (CPUParticlesGizmoPlugin, ParticlesGizmoPlugin)
 *   - Reflection probes (ReflectionProbeGizmoPlugin)
 *   - Global illumination probes (GIProbeGizmoPlugin)
 *   - Collision shapes (CollisionShapeSpatialGizmoPlugin,
 * CollisionPolygonSpatialGizmoPlugin)
 *   - Navigation meshes (NavigationMeshSpatialGizmoPlugin)
 *   - Physics joints (JointSpatialGizmoPlugin)
 *
 * Additional utility classes:
 *   - JointGizmosDrawer: Utility class for drawing physics joint visualizations
 *
 * @see EditorSpatialGizmoPlugin
 * @see EditorSpatialGizmo
 * @see spatial_editor_plugin.h
 */
#ifndef SPATIAL_EDITOR_GIZMOS_H
#define SPATIAL_EDITOR_GIZMOS_H

#include "editor/plugins/spatial_editor_plugin.h"
#include "scene/3d/camera.h"

class Camera;

class LightSpatialGizmoPlugin : public EditorSpatialGizmoPlugin {

  GDCLASS(LightSpatialGizmoPlugin, EditorSpatialGizmoPlugin);

public:
  bool has_gizmo(Spatial *p_spatial);
  String get_name() const;
  int get_priority() const;

  String get_handle_name(const EditorSpatialGizmo *p_gizmo, int p_idx) const;
  Variant get_handle_value(EditorSpatialGizmo *p_gizmo, int p_idx) const;
  void set_handle(EditorSpatialGizmo *p_gizmo, int p_idx, Camera *p_camera,
                  const Point2 &p_point);
  void commit_handle(EditorSpatialGizmo *p_gizmo, int p_idx,
                     const Variant &p_restore, bool p_cancel = false);
  void redraw(EditorSpatialGizmo *p_gizmo);

  LightSpatialGizmoPlugin();
};

class AudioStreamPlayer3DSpatialGizmoPlugin : public EditorSpatialGizmoPlugin {

  GDCLASS(AudioStreamPlayer3DSpatialGizmoPlugin, EditorSpatialGizmoPlugin);

public:
  bool has_gizmo(Spatial *p_spatial);
  String get_name() const;
  int get_priority() const;

  String get_handle_name(const EditorSpatialGizmo *p_gizmo, int p_idx) const;
  Variant get_handle_value(EditorSpatialGizmo *p_gizmo, int p_idx) const;
  void set_handle(EditorSpatialGizmo *p_gizmo, int p_idx, Camera *p_camera,
                  const Point2 &p_point);
  void commit_handle(EditorSpatialGizmo *p_gizmo, int p_idx,
                     const Variant &p_restore, bool p_cancel = false);
  void redraw(EditorSpatialGizmo *p_gizmo);

  AudioStreamPlayer3DSpatialGizmoPlugin();
};

class CameraSpatialGizmoPlugin : public EditorSpatialGizmoPlugin {

  GDCLASS(CameraSpatialGizmoPlugin, EditorSpatialGizmoPlugin);

public:
  bool has_gizmo(Spatial *p_spatial);
  String get_name() const;
  int get_priority() const;

  String get_handle_name(const EditorSpatialGizmo *p_gizmo, int p_idx) const;
  Variant get_handle_value(EditorSpatialGizmo *p_gizmo, int p_idx) const;
  void set_handle(EditorSpatialGizmo *p_gizmo, int p_idx, Camera *p_camera,
                  const Point2 &p_point);
  void commit_handle(EditorSpatialGizmo *p_gizmo, int p_idx,
                     const Variant &p_restore, bool p_cancel = false);
  void redraw(EditorSpatialGizmo *p_gizmo);

  CameraSpatialGizmoPlugin();
};

class MeshInstanceSpatialGizmoPlugin : public EditorSpatialGizmoPlugin {

  GDCLASS(MeshInstanceSpatialGizmoPlugin, EditorSpatialGizmoPlugin);

public:
  bool has_gizmo(Spatial *p_spatial);
  String get_name() const;
  int get_priority() const;
  bool can_be_hidden() const;
  void redraw(EditorSpatialGizmo *p_gizmo);

  MeshInstanceSpatialGizmoPlugin();
};

class Sprite3DSpatialGizmoPlugin : public EditorSpatialGizmoPlugin {

  GDCLASS(Sprite3DSpatialGizmoPlugin, EditorSpatialGizmoPlugin);

public:
  bool has_gizmo(Spatial *p_spatial);
  String get_name() const;
  int get_priority() const;
  bool can_be_hidden() const;
  void redraw(EditorSpatialGizmo *p_gizmo);

  Sprite3DSpatialGizmoPlugin();
};

class Position3DSpatialGizmoPlugin : public EditorSpatialGizmoPlugin {

  GDCLASS(Position3DSpatialGizmoPlugin, EditorSpatialGizmoPlugin);

  Ref<ArrayMesh> pos3d_mesh;
  Vector<Vector3> cursor_points;

public:
  bool has_gizmo(Spatial *p_spatial);
  String get_name() const;
  int get_priority() const;
  void redraw(EditorSpatialGizmo *p_gizmo);

  Position3DSpatialGizmoPlugin();
};

class SkeletonSpatialGizmoPlugin : public EditorSpatialGizmoPlugin {

  GDCLASS(SkeletonSpatialGizmoPlugin, EditorSpatialGizmoPlugin);

public:
  bool has_gizmo(Spatial *p_spatial);
  String get_name() const;
  int get_priority() const;
  void redraw(EditorSpatialGizmo *p_gizmo);

  SkeletonSpatialGizmoPlugin();
};

class PhysicalBoneSpatialGizmoPlugin : public EditorSpatialGizmoPlugin {

  GDCLASS(PhysicalBoneSpatialGizmoPlugin, EditorSpatialGizmoPlugin);

public:
  bool has_gizmo(Spatial *p_spatial);
  String get_name() const;
  int get_priority() const;
  void redraw(EditorSpatialGizmo *p_gizmo);

  PhysicalBoneSpatialGizmoPlugin();
};

class RayCastSpatialGizmoPlugin : public EditorSpatialGizmoPlugin {

  GDCLASS(RayCastSpatialGizmoPlugin, EditorSpatialGizmoPlugin);

public:
  bool has_gizmo(Spatial *p_spatial);
  String get_name() const;
  int get_priority() const;
  void redraw(EditorSpatialGizmo *p_gizmo);

  RayCastSpatialGizmoPlugin();
};

class SpringArmSpatialGizmoPlugin : public EditorSpatialGizmoPlugin {

  GDCLASS(SpringArmSpatialGizmoPlugin, EditorSpatialGizmoPlugin);

public:
  bool has_gizmo(Spatial *p_spatial);
  String get_name() const;
  int get_priority() const;
  void redraw(EditorSpatialGizmo *p_gizmo);

  SpringArmSpatialGizmoPlugin();
};

class VehicleWheelSpatialGizmoPlugin : public EditorSpatialGizmoPlugin {

  GDCLASS(VehicleWheelSpatialGizmoPlugin, EditorSpatialGizmoPlugin);

public:
  bool has_gizmo(Spatial *p_spatial);
  String get_name() const;
  int get_priority() const;
  void redraw(EditorSpatialGizmo *p_gizmo);

  VehicleWheelSpatialGizmoPlugin();
};

class SoftBodySpatialGizmoPlugin : public EditorSpatialGizmoPlugin {

  GDCLASS(SoftBodySpatialGizmoPlugin, EditorSpatialGizmoPlugin);

public:
  bool has_gizmo(Spatial *p_spatial);
  String get_name() const;
  int get_priority() const;
  bool is_selectable_when_hidden() const;
  void redraw(EditorSpatialGizmo *p_gizmo);

  String get_handle_name(const EditorSpatialGizmo *p_gizmo, int p_idx) const;
  Variant get_handle_value(EditorSpatialGizmo *p_gizmo, int p_idx) const;
  void commit_handle(EditorSpatialGizmo *p_gizmo, int p_idx,
                     const Variant &p_restore, bool p_cancel);
  bool is_handle_highlighted(const EditorSpatialGizmo *p_gizmo, int idx) const;

  SoftBodySpatialGizmoPlugin();
};

class VisibilityNotifierGizmoPlugin : public EditorSpatialGizmoPlugin {

  GDCLASS(VisibilityNotifierGizmoPlugin, EditorSpatialGizmoPlugin);

public:
  bool has_gizmo(Spatial *p_spatial);
  String get_name() const;
  int get_priority() const;
  void redraw(EditorSpatialGizmo *p_gizmo);

  String get_handle_name(const EditorSpatialGizmo *p_gizmo, int p_idx) const;
  Variant get_handle_value(EditorSpatialGizmo *p_gizmo, int p_idx) const;
  void set_handle(EditorSpatialGizmo *p_gizmo, int p_idx, Camera *p_camera,
                  const Point2 &p_point);
  void commit_handle(EditorSpatialGizmo *p_gizmo, int p_idx,
                     const Variant &p_restore, bool p_cancel = false);

  VisibilityNotifierGizmoPlugin();
};

class CPUParticlesGizmoPlugin : public EditorSpatialGizmoPlugin {
  GDCLASS(CPUParticlesGizmoPlugin, EditorSpatialGizmoPlugin);

public:
  bool has_gizmo(Spatial *p_spatial);
  String get_name() const;
  int get_priority() const;
  bool is_selectable_when_hidden() const;
  void redraw(EditorSpatialGizmo *p_gizmo);
  CPUParticlesGizmoPlugin();
};

class ParticlesGizmoPlugin : public EditorSpatialGizmoPlugin {

  GDCLASS(ParticlesGizmoPlugin, EditorSpatialGizmoPlugin);

public:
  bool has_gizmo(Spatial *p_spatial);
  String get_name() const;
  int get_priority() const;
  bool is_selectable_when_hidden() const;
  void redraw(EditorSpatialGizmo *p_gizmo);

  String get_handle_name(const EditorSpatialGizmo *p_gizmo, int p_idx) const;
  Variant get_handle_value(EditorSpatialGizmo *p_gizmo, int p_idx) const;
  void set_handle(EditorSpatialGizmo *p_gizmo, int p_idx, Camera *p_camera,
                  const Point2 &p_point);
  void commit_handle(EditorSpatialGizmo *p_gizmo, int p_idx,
                     const Variant &p_restore, bool p_cancel = false);

  ParticlesGizmoPlugin();
};

class ReflectionProbeGizmoPlugin : public EditorSpatialGizmoPlugin {

  GDCLASS(ReflectionProbeGizmoPlugin, EditorSpatialGizmoPlugin);

public:
  bool has_gizmo(Spatial *p_spatial);
  String get_name() const;
  int get_priority() const;
  void redraw(EditorSpatialGizmo *p_gizmo);

  String get_handle_name(const EditorSpatialGizmo *p_gizmo, int p_idx) const;
  Variant get_handle_value(EditorSpatialGizmo *p_gizmo, int p_idx) const;
  void set_handle(EditorSpatialGizmo *p_gizmo, int p_idx, Camera *p_camera,
                  const Point2 &p_point);
  void commit_handle(EditorSpatialGizmo *p_gizmo, int p_idx,
                     const Variant &p_restore, bool p_cancel = false);

  ReflectionProbeGizmoPlugin();
};

class GIProbeGizmoPlugin : public EditorSpatialGizmoPlugin {

  GDCLASS(GIProbeGizmoPlugin, EditorSpatialGizmoPlugin);

public:
  bool has_gizmo(Spatial *p_spatial);
  String get_name() const;
  int get_priority() const;
  void redraw(EditorSpatialGizmo *p_gizmo);

  String get_handle_name(const EditorSpatialGizmo *p_gizmo, int p_idx) const;
  Variant get_handle_value(EditorSpatialGizmo *p_gizmo, int p_idx) const;
  void set_handle(EditorSpatialGizmo *p_gizmo, int p_idx, Camera *p_camera,
                  const Point2 &p_point);
  void commit_handle(EditorSpatialGizmo *p_gizmo, int p_idx,
                     const Variant &p_restore, bool p_cancel = false);

  GIProbeGizmoPlugin();
};

#if 0
class BakedIndirectLightGizmoPlugin : public EditorSpatialGizmoPlugin {

	GDCLASS(BakedIndirectLightGizmoPlugin, EditorSpatialGizmoPlugin);

public:
	bool has_gizmo(Spatial *p_spatial);
	String get_name() const;
	int get_priority() const;
	void redraw(EditorSpatialGizmo *p_gizmo);

	String get_handle_name(const EditorSpatialGizmo *p_gizmo, int p_idx) const;
	Variant get_handle_value(EditorSpatialGizmo *p_gizmo, int p_idx) const;
	void set_handle(EditorSpatialGizmo *p_gizmo, int p_idx, Camera *p_camera, const Point2 &p_point);
	void commit_handle(EditorSpatialGizmo *p_gizmo, int p_idx, const Variant &p_restore, bool p_cancel = false);

	BakedIndirectLightGizmoPlugin();
};
#endif
class CollisionShapeSpatialGizmoPlugin : public EditorSpatialGizmoPlugin {

  GDCLASS(CollisionShapeSpatialGizmoPlugin, EditorSpatialGizmoPlugin);

public:
  bool has_gizmo(Spatial *p_spatial);
  String get_name() const;
  int get_priority() const;
  void redraw(EditorSpatialGizmo *p_gizmo);

  String get_handle_name(const EditorSpatialGizmo *p_gizmo, int p_idx) const;
  Variant get_handle_value(EditorSpatialGizmo *p_gizmo, int p_idx) const;
  void set_handle(EditorSpatialGizmo *p_gizmo, int p_idx, Camera *p_camera,
                  const Point2 &p_point);
  void commit_handle(EditorSpatialGizmo *p_gizmo, int p_idx,
                     const Variant &p_restore, bool p_cancel = false);

  CollisionShapeSpatialGizmoPlugin();
};

class CollisionPolygonSpatialGizmoPlugin : public EditorSpatialGizmoPlugin {
  GDCLASS(CollisionPolygonSpatialGizmoPlugin, EditorSpatialGizmoPlugin);

public:
  bool has_gizmo(Spatial *p_spatial);
  String get_name() const;
  int get_priority() const;
  void redraw(EditorSpatialGizmo *p_gizmo);
  CollisionPolygonSpatialGizmoPlugin();
};

class NavigationMeshSpatialGizmoPlugin : public EditorSpatialGizmoPlugin {

  GDCLASS(NavigationMeshSpatialGizmoPlugin, EditorSpatialGizmoPlugin);

  struct _EdgeKey {

    Vector3 from;
    Vector3 to;

    bool operator<(const _EdgeKey &p_with) const {
      return from == p_with.from ? to < p_with.to : from < p_with.from;
    }
  };

public:
  bool has_gizmo(Spatial *p_spatial);
  String get_name() const;
  int get_priority() const;
  void redraw(EditorSpatialGizmo *p_gizmo);

  NavigationMeshSpatialGizmoPlugin();
};

class JointGizmosDrawer {
public:
  static Basis look_body(const Transform &p_joint_transform,
                         const Transform &p_body_transform);
  static Basis look_body_toward(Vector3::Axis p_axis,
                                const Transform &joint_transform,
                                const Transform &body_transform);
  static Basis look_body_toward_x(const Transform &p_joint_transform,
                                  const Transform &p_body_transform);
  static Basis look_body_toward_y(const Transform &p_joint_transform,
                                  const Transform &p_body_transform);
  /// Special function just used for physics joints, it returns a basis
  /// constrained toward Joint Z axis with axis X and Y that are looking toward
  /// the body and oriented toward up
  static Basis look_body_toward_z(const Transform &p_joint_transform,
                                  const Transform &p_body_transform);

  // Draw circle around p_axis
  static void draw_circle(Vector3::Axis p_axis, real_t p_radius,
                          const Transform &p_offset, const Basis &p_base,
                          real_t p_limit_lower, real_t p_limit_upper,
                          Vector<Vector3> &r_points, bool p_inverse = false);
  static void draw_cone(const Transform &p_offset, const Basis &p_base,
                        real_t p_swing, real_t p_twist,
                        Vector<Vector3> &r_points);
};

class JointSpatialGizmoPlugin : public EditorSpatialGizmoPlugin {

  GDCLASS(JointSpatialGizmoPlugin, EditorSpatialGizmoPlugin);

public:
  bool has_gizmo(Spatial *p_spatial);
  String get_name() const;
  int get_priority() const;
  void redraw(EditorSpatialGizmo *p_gizmo);

  static void CreatePinJointGizmo(const Transform &p_offset,
                                  Vector<Vector3> &r_cursor_points);
  static void CreateHingeJointGizmo(
      const Transform &p_offset, const Transform &p_trs_joint,
      const Transform &p_trs_body_a, const Transform &p_trs_body_b,
      real_t p_limit_lower, real_t p_limit_upper, bool p_use_limit,
      Vector<Vector3> &r_common_points, Vector<Vector3> *r_body_a_points,
      Vector<Vector3> *r_body_b_points);
  static void CreateSliderJointGizmo(
      const Transform &p_offset, const Transform &p_trs_joint,
      const Transform &p_trs_body_a, const Transform &p_trs_body_b,
      real_t p_angular_limit_lower, real_t p_angular_limit_upper,
      real_t p_linear_limit_lower, real_t p_linear_limit_upper,
      Vector<Vector3> &r_points, Vector<Vector3> *r_body_a_points,
      Vector<Vector3> *r_body_b_points);
  static void CreateConeTwistJointGizmo(const Transform &p_offset,
                                        const Transform &p_trs_joint,
                                        const Transform &p_trs_body_a,
                                        const Transform &p_trs_body_b,
                                        real_t p_swing, real_t p_twist,
                                        Vector<Vector3> *r_body_a_points,
                                        Vector<Vector3> *r_body_b_points);
  static void CreateGeneric6DOFJointGizmo(
      const Transform &p_offset, const Transform &p_trs_joint,
      const Transform &p_trs_body_a, const Transform &p_trs_body_b,
      real_t p_angular_limit_lower_x, real_t p_angular_limit_upper_x,
      real_t p_linear_limit_lower_x, real_t p_linear_limit_upper_x,
      bool p_enable_angular_limit_x, bool p_enable_linear_limit_x,
      real_t p_angular_limit_lower_y, real_t p_angular_limit_upper_y,
      real_t p_linear_limit_lower_y, real_t p_linear_limit_upper_y,
      bool p_enable_angular_limit_y, bool p_enable_linear_limit_y,
      real_t p_angular_limit_lower_z, real_t p_angular_limit_upper_z,
      real_t p_linear_limit_lower_z, real_t p_linear_limit_upper_z,
      bool p_enable_angular_limit_z, bool p_enable_linear_limit_z,
      Vector<Vector3> &r_points, Vector<Vector3> *r_body_a_points,
      Vector<Vector3> *r_body_b_points);

  JointSpatialGizmoPlugin();
};

#endif // SPATIAL_EDITOR_GIZMOS_H
