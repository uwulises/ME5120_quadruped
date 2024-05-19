import mujoco
import mujoco.viewer
import time

model = mujoco.MjModel.from_xml_path("model/scene.xml")
mujoco.MjvScene(model, maxgeom=10)
data = mujoco.MjData(model)
model.opt.gravity = (0, 0, -9.81)


mujoco.mj_step(model, data)

# Make renderer, render and show the pixels
renderer = mujoco.Renderer(model)
renderer.update_scene(data)    


with mujoco.viewer.launch(model, data, show_left_ui=True,show_right_ui=True) as viewer:
  # Close the viewer automatically after 30 wall-seconds.
  start = time.time()
  while viewer.is_running():
    step_start = time.time()

    # mj_step can be replaced with code that also evaluates
    # a policy and applies a control signal before stepping the physics.
    mujoco.mj_step(model, data)

    # Example modification of a viewer option: toggle contact points every two seconds.
    with viewer.lock():
      viewer.opt.flags[mujoco.mjtVisFlag.mjVIS_CONTACTPOINT] = int(data.time % 2)

    # Pick up changes to the physics state, apply perturbations, update options from GUI.
    viewer.sync()

    # Rudimentary time keeping, will drift relative to wall clock.
    time_until_next_step = model.opt.timestep - (time.time() - step_start)
    if time_until_next_step > 0:
      time.sleep(time_until_next_step)