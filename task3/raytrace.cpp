// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "objects.hpp"
#include "raytrace.hpp"
#include "color.hpp"
#include "ray.hpp"

Color Raytrace::BACKGROUND =  {0.2f, 0.7f, 0.8f};
#define _GET_RETURN_INTERSECT(_min_dist, _obj_pointer, _function_call) \
                                                                       \
    sf::Vector2f _returned_vector = _function_call;                    \
    _min_dist = _returned_vector.x;                                    \
    union                                                              \
    {                                                                  \
        float fl;                                                      \
        const Drawable *ptr;                                           \
    } _temp;                                                           \
    _temp.fl = _returned_vector.y;                                     \
    _obj_pointer = _temp.ptr;


Color Raytrace::ray_cast(Ray& ray, float t_min, float t_max, const ObjectManager &objects, const LightManager &lights)
{
    float min_dist = {};
    const Drawable *closest_obj = NULL;

    _GET_RETURN_INTERSECT(min_dist, closest_obj, ray.closestIntersection(t_min, t_max, objects));

    if (closest_obj == NULL)
        return BACKGROUND;
    
    // //if(closest_obj)
    //     {
    //         fprintf(stderr, "closest_obj %p\n", closest_obj);
    //         fflush(0);
    //     }

    sf::Vector3f point = ray.getPoint(min_dist);            // point intersection
    sf::Vector3f normal = closest_obj->getNormal(point);    // normal at point intersection

    normal = normalize(normal);
    
    //local color
    ray.m_origin = point;
    ray.m_direction = -ray.m_direction;
    Color local_color = closest_obj->getColor() * ComputeLighting(ray, normal, closest_obj->m_properties, objects, lights);

    //reflection
    float r = closest_obj->m_properties.reflective;

    if (ray.m_generation <= 0 || r <= 0)
        return local_color;

    ray.reflect(normal);
    ray.m_origin = point;
    ray.m_generation--;

    Color reflected_color = ray_cast(ray, 0.001f, _INFINITY, objects, lights);

    return local_color * (1 - r) + reflected_color * r;
}

float Raytrace::ComputeLighting(Ray &ray, const sf::Vector3f &normal, Material prop, const ObjectManager &objects, const LightManager &lights)
{
    float intensity = 0;

    for(size_t i = 0; i < lights.size(); i ++)
        if (lights[i].m_type == Light::Type::AMBIENT)
            intensity += lights[i].m_intensity;
        else
        {   
            sf::Vector3f L = {};
            float t_max = {};
            if (lights[i].m_type == Light::Type::POINT)
            {
                L = lights[i].m_position - ray.m_origin;
                t_max = 1;
            }
            else
            {
                L = -lights[i].m_direction;
                t_max = _INFINITY;
            }


            //check for shadows
            const Drawable* shadow_obj = nullptr;
            float shadow_obj_dist = {};
            
            Ray ray_to_light {ray.m_origin, L};
            _GET_RETURN_INTERSECT(shadow_obj_dist, shadow_obj, ray_to_light.closestIntersection(0.001f, t_max, objects));

            if(shadow_obj != nullptr)
                continue;

            //diffuse
            float cos_alpha = dot(ray_to_light.m_direction, normal) / (length(ray_to_light.m_direction) * length(normal));
            //fprintf(stderr, "cos-alpha is %f\n", cos_alpha);
            
            if(cos_alpha > 0)
                intensity += lights[i].m_intensity * cos_alpha;  

            //specular
            if( prop.specular != -1)
            {   
                ray_to_light.reflect(normal);
                float cos_beta = dot(normalize(ray_to_light.m_direction), normalize(ray.m_direction));
                
                if(cos_beta > 0)
                    intensity += lights[i].m_intensity * powf(cos_beta , static_cast<float>(prop.specular));
            }
        }
    return intensity;
}