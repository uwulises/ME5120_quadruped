# ME5120_quadruped
Este proyecto pretende diseñar y fabricar un cuadrúpedo autónomo. La primera etapa corresponde a seleccionar y diseñar el hardware adecuado para montar una estructura funcional.
Las siguientes etapas se centrarán en el control de la caminata y planificación/navegación del entorno.

## TODO
- [x] PCB design
- [x] Basic motor control
- [ ] SPI communication
- [ ] MQTT wireless moves callback
- [ ] MuJoCo model update


# Design

<img src="/assembly.png" width="500">

# Hardware

## Driver + Rpi Pico Hat
En esta versión se propone utilizar el driver texas drv8302 junto a la [librería SimpleFOC](https://docs.simplefoc.com/drv8302_example)
### SimpleFOC example
<img src="https://docs.simplefoc.com/extras/Images/drv8302_arduino_connection.jpg" width="500" height="200" />

### Hat schematic
 
<img src="/wiring.png" width="400">

### Hat Board

<img src="/board_hat.png" width="400">

## Simulation - Mujoco
<img src="/sim_mujoco.png" width="500">
