# craytrace
A very rudimentary raytracer written in C

Follows [RayTracing in One Weekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html) up until section 10.

# TODO
If I ever revisit this, it needs a full re-write. The early design decisions I made meant that creating materials would be messy and difficult so:
- [ ] Rewrite with considerations for adding materials.
- [ ] Consider whether dynamic memory allocation for object arrays is necessary, setting a sensible static cap may be better in terms of performance?
