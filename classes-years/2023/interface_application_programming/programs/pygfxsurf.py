import pygfx as gfx
import numpy as np

l = 15.5
x = np.arange(-l,l,0.2)
y = np.arange(-l,l,0.2)
N = x.size
(x,y) = np.meshgrid(x,y)
r = np.sqrt(x**2+y**2)
r = np.reshape(r,N*N)

d = 100
scene = gfx.Scene()
surface = gfx.Mesh(
   gfx.plane_geometry(2*d,2*d,N-1,N-1),
   gfx.MeshStandardMaterial(color="#DDBB22",roughness=0.5,metalness=0.5,flat_shading=True),
   )
surface.rotation.multiply(
   gfx.linalg.Quaternion().set_from_euler(
   gfx.linalg.Euler(-np.pi/2,0,0)))
scene.add(surface)
scene.add(gfx.AmbientLight(intensity=1))
light = gfx.objects.PointLight()
light.position.set(d,d,d)
scene.add(light)

k = 0
dk = 0.01
kmax = 1
dt = 0.01
def animate():
   global k,dk
   k += dk
   if ((k > kmax) | (k < 0)):
      dk = -dk
   surface.rotation.multiply(
      gfx.linalg.Quaternion().set_from_euler(
      gfx.linalg.Euler(0,0,dt)))
   surface.geometry.positions.data[:,2] = d*np.sin(k*r)/(k*r)
   surface.geometry.positions.update_range(0,N*N)

gfx.show(scene,before_render=animate)
