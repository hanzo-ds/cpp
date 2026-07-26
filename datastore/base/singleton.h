#pragma once

namespace datastore {

template <typename T>
T* Singleton() {
    static T instance;
    return &instance;
}

}
