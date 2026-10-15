#!/usr/bin/env python

################################################################################
# Given a potential f and a pressure fild p, this code return an exact solution
# for the Navier Stokes Equations and the relative forcing term.
################################################################################
from sympy import *
import sys
pde_list=["stokes","dynamic_stokes","stationary_naviers-stokes","navier-stokes"]

bar="========================================================================"
x,y,t,k = symbols('x y t k')

if len(sys.argv)<4 :
  print bar
  print " Two arguments are required"
  print "  Usage: " + sys.argv[0] + " [potential] [pressure] [equation]"
  print " Equation:"
  print "  - stokes"
  print "  - dynamic_stokes"
  print "  - navier-stokes"
  print "  - stationary_naviers-stokes"
  print bar
  exit(0)

# Potential:
f = sys.argv[1]
# Pressure:
p = sys.argv[2]
#equation
eq = sys.argv[3]
if eq not in pde_list :
  print bar
  print str(eq) + " is not in " + str(pde_list)
  print bar
  exit(0)

u      = [diff(f,y),-diff(f,x)]
divu   = diff(u[0],x)+diff(u[1],y)
ut     = [diff(u[0],t),diff(u[1],t)]
ugradu = [u[0]*diff(u[0],x)+u[1]*diff(u[0],y) ,
         u[0]*diff(u[1],x)+u[1]*diff(u[1],y) ]
gradu  = [[diff(u[0],x),diff(u[0],y)],[diff(u[1],x),diff(u[1],y)]]
gradut = [[diff(u[0],x),diff(u[1],x)],[diff(u[0],y),diff(u[1],y)]]
lapl   = [diff(u[0],x,2)+diff(u[0],y,2),diff(u[1],x,2)+diff(u[1],y,2) ]
lapls  = [lapl[0]/2 + diff(diff(u[0],x)+diff(u[1],y) ,x)/2, lapl[1]/2 + diff(diff(u[0],x)+diff(u[1],y) ,y)/2  ]
gradp  = [diff(p,x),diff(p,y)]

if eq == pde_list[0]:
  forcing=[-k*lapls[i]+gradp[i] for i in xrange(2)]
if eq == pde_list[1]:
  forcing=[ut[i]-k*lapls[i]+gradp[i] for i in xrange(2)]
if eq == pde_list[2]:
  forcing=[ugradu[i]-k*lapls[i]+gradp[i] for i in xrange(2)]
if eq == pde_list[3]:
  forcing=[ut[i]+ugradu[i]-k*lapls[i]+gradp[i] for i in xrange(2)]

print "========================================================================"
print " Legend: "
print "  - x,y,t = variables "
print "  - k     = viscosity "
print "========================================================================"
print " Potential:       " + str(f)
print "========================================================================"
print " Pressure:        " + str(p)
print "========================================================================"
print " div(u):          " + str(divu)
print "========================================================================"
print " grad p:          " + str(gradp)
print "========================================================================"
print " grad u:          " + str(gradu)
print "========================================================================"
print " (grad u)^t:      " + str(gradut)
print "========================================================================"
print " u_t:             " + str(ut)
print "========================================================================"
print " div(grad u):     " + str(lapl)
print "========================================================================"
print " div((grad u + (grad u)^2)/2): " + str(lapls)
print "========================================================================"
print " (grad u)u:       " + str(ugradu)
print "========================================================================"
print " Exact solution:  " + str(u)
print "========================================================================"
print " Forcing term:    " + str(forcing)
print "========================================================================"
