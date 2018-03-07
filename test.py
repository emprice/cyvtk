import cyvtk
import numpy as np

r = np.linspace(0., 16./15., 129)
mu = np.linspace(-1., 1., 33)
phi = np.linspace(0., 2.*np.pi, 65)
theta = np.arccos(mu)

rr, muu, phii = np.meshgrid(r, mu, phi, indexing='ij')
thetaa = np.arccos(muu)

xx = rr * np.sin(thetaa) * np.cos(phii)
yy = rr * np.sin(thetaa) * np.sin(phii)
zz = rr * np.cos(thetaa)

ff = xx**2 + yy**3 - zz

vx = xx**2
vy = yy**3
vz = zz

c = cyvtk.PyStructuredGrid(r, theta, phi)
c.add_scalar_point_data('foo', ff)
c.add_vector_point_data('bar', vx, vy, vz)
c.write_to_file('baz')

c = cyvtk.PyUnstructuredGrid(xx.ravel(), yy.ravel(), zz.ravel())
c.add_scalar_point_data('foo', ff.ravel())
c.add_vector_point_data('bar', vx.ravel(), vy.ravel(), vz.ravel())
c.write_to_file('qux')
