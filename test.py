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

ff = np.where(xx**2 + (yy / 0.3)**2 + (zz / 0.6)**2 <= 1., 1., 0.)

c = cyvtk.PyStructuredGrid(r, theta, phi)
c.add_scalar_point_data('foo', ff)
c.write_to_file('hello')
