#pragma once

#include <memory>
#include <string>
#include <vector>

#include <glm/glm.hpp>

#include "common.hpp"

/**
 * This class works as a base to be inherited from to implement any type of object.
 */
class LIB_API Object
{
public:
    Object();
    Object(const std::string name);

    int get_id() const;

    void add_child(const std::shared_ptr<Object> new_child);
    std::vector<std::shared_ptr<Object>> get_children() const;

    std::string get_name() const;
    void set_name(const std::string new_name);

    virtual glm::mat4 get_local_matrix() const;
    virtual int get_priority() const;

    virtual void render(const glm::mat4 world_matrix) const = 0;
protected:
    std::vector<std::shared_ptr<Object>> children;
private:
    static int next_id;

    int id;
    std::string name;
};
