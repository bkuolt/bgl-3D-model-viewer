#include "gl.hpp"


namespace bgl {

struct BoundingBox {
   struct Bound {
      float min;
      float max;
   };

   const vec3& getSize() const noexcept {
       return _dimensions = {
            _bounds.x.max -  _bounds.x.min,
            _bounds.y.max -  _bounds.y.min,
            _bounds.z.max -  _bounds.z.min
         };
   }


   void resize(const vec3 &dimensions) {
      _dimensions = dimensions;
   }

   mutable glm::vec3 _dimensions;
   glm::tvec3<Bound> _bounds;
};

}  // namespace bgl