#
# taichisurf.py
# Neil Gershenfeld 3/1/23
# Taichi particle plot example
#
import taichi as ti
ti.init()

N = 100
r = 1/N
kmax = 50
dk = 0.05
k = 0.0

p = ti.Vector.field(3,float,N*N)
c = ti.Vector.field(3,float,N*N)

@ti.kernel
def update(k:float):
   for i in range(N):
      for j in range(N):
         x = i/(N-1)-0.5
         y = j/(N-1)-0.5
         r = ti.sqrt(x*x+y*y)
         z = ti.sin(k*r)/(k*r)
         p[i+N*j].z = z
         c[i+N*j] = (z,1-z,0)

@ti.kernel
def init():
   for i in range(N):
      for j in range(N):
         p[i+N*j].x = i/N
         p[i+N*j].y = j/N

i = 0

def render():
    global k,dk
    scene.set_camera(camera)
    scene.point_light(pos=(0.5,1.5,0.5), color=(0.5,0.5,0.5))
    scene.point_light(pos=(0.5,1.5,1.5), color=(0.5,0.5,0.5))
    k += dk
    if ((k >= kmax) | (k <= 0)):
      dk = -dk
    update(k)
    scene.particles(p,radius=r,per_vertex_color=c)
    canvas.scene(scene)

window = ti.ui.Window("sin(r)/r",(1080,720))
canvas = window.get_canvas()
gui = window.get_gui()
scene = ti.ui.Scene()
camera = ti.ui.Camera()
camera.position(6,2.5,2)
camera.lookat(0.5,0.5,0.3)
camera.up(0,0,1)
camera.fov(15)

init()

count = 0
while window.running:
   render()
   #window.save_image(f'images/out{count:05d}.png')
   count += 1
   window.show()

