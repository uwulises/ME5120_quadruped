import mujoco
import mujoco.viewer
import time
import numpy as np

model = mujoco.MjModel.from_xml_path("model/scene.xml")
data = mujoco.MjData(model)
model.opt.gravity = (0, 0, -9.81)
mujoco.mj_resetDataKeyframe(model, data, 0)
with mujoco.viewer.launch_passive(model, data, show_left_ui=True,show_right_ui=True) as viewer:
  start = time.time()
  while viewer.is_running():
    step_start = time.time()
    data.ctrl = np.random.randn(model.nu)
    mujoco.mj_step(model, data)
    # Pick up changes to the physics state, apply perturbations, update options from GUI.
    viewer.sync()
    # Rudimentary time keeping, will drift relative to wall clock.
    time_until_next_step = model.opt.timestep - (time.time() - step_start)
    if time_until_next_step > 0:
      time.sleep(time_until_next_step/2)