#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>

using namespace std;


// ===============================================================================================
// PARENT CLASS SHAPE (VIRTUAL INTERFACE TO COMMUNICATE BETWEEN MAIN AND SHAPE DEFINATION CLASS)
// ===============================================================================================


class Shape {
public:
    virtual ~Shape() = default;
    virtual void describe() const = 0;
    virtual void compute() const = 0;

    static unique_ptr<Shape> create(int argc, char* argv[]);
};

//
// =========================================================================================
// ACTUAL DEFINATION OF SHAPE CLASSE THAT DESCRIBES AND COMPUTES SHAPE PARMETERS
// =========================================================================================
//

class  ShapeDef : public Shape {
    string name;
    int required_params;
    vector<double> params;

    function<void()> describe_fn;                               // function data member for description
    function<double(const vector<double>&)> perimeter_fn;       // function data member for perimeter
    function<double(const vector<double>&)> area_fn;            // function data member for area

public:
     ShapeDef(string n,
                 int req,
                 int argc,
                 char* argv[],
                 function<void()> d,
                 function<double(const vector<double>&)> p,
                 function<double(const vector<double>&)> a)
        : name(n),
          required_params(req),
          describe_fn(d),
          perimeter_fn(p),
          area_fn(a)
    {   
        if (argc - 2 < required_params && argc - 2 > 0) {
            cout << "Warning: " << name << " requires " << required_params << " parameters.\n";  // Warning in case of less parameters than required
        }

        for (int i = 2; i < argc && params.size() < required_params; ++i)
            try { 
                params.push_back(stod(argv[i]));
            } catch (const invalid_argument& e) {
                cout << "Warning: Invalid parameter '" << argv[i] << "' ignored.\n";
            }

        if (argc - 2 > required_params)
            cout << "Warning: Extra parameters ignored.\n";                                     // Warning in case of more parameters than required
    }

    void describe() const override {
        describe_fn();
    }

    void compute() const override {
        if (params.size() < required_params)
            return;

        cout << "Perimeter: " << perimeter_fn(params) << "\n";
        cout << "Area: " << area_fn(params) << "\n";
    }
};

//
// ======================================================
// REGISTRY FOR SHAPE CLASS CREATORS
// ======================================================
//

using Creator = function<unique_ptr<Shape>(int, char**)>;

static unordered_map<string, Creator>& registry() {
    static unordered_map<string, Creator> r;
    return r;
}

unique_ptr<Shape> Shape::create(int argc, char* argv[]) {
    if (registry().find(argv[1]) == registry().end()) {
        cout << "Error: Unknown shape '" << argv[1] << "'\n";
        return nullptr;
    }
    return registry().at(argv[1])(argc, argv);
}

//
// =====================================================
// SHAPE REGISTRATIONS 
// ====================================================
//  TEMPALTE 
//
// static bool REG_SHAPE = [] {
//     registry()["SHAPE_NAME"] = [](int argc, char* argv[]) {
//         return make_unique< ShapeDef>(
//             "SHAPE_NAME",
//             REQUIRED_PARAMETER_COUNT,
//             argc,
//             argv,
//             [] { cout << "CUSTOM_DESCRIPTION\n"; },
//             [](const auto& p) { return PERIMETER_FORMULA; },
//             [](const auto& p) { return AREA_FORMULA; }
//         );
//     };
//     return true;
// }();
//
// ======================================================
//

// ---- Circle ----
static bool reg_circle = [] {
    registry()["circle"] = [](int argc, char* argv[]) {
        return make_unique< ShapeDef>(
            "circle",
            1,
            argc,
            argv,
            [] { cout << "A circle has a radius.\n"; },
            [](const auto& p) { return 2 * 3.14159 * p[0]; },
            [](const auto& p) { return 3.14159 * p[0] * p[0]; }
        );
    };
    return true;
}();

// ---- Square ----
static bool reg_square = [] {
    registry()["square"] = [](int argc, char* argv[]) {
        return make_unique< ShapeDef>(
            "square",
            1,
            argc,
            argv,
            [] { cout << "A square has four sides that are of equal length.\n"; },
            [](const auto& p) { return 4 * p[0]; },
            [](const auto& p) { return p[0] * p[0]; }
        );
    };
    return true;
}();

// ---- Rectangle ----
static bool reg_rect = [] {
    registry()["rect"] = [](int argc, char* argv[]) {
        return make_unique< ShapeDef>(
            "rectangle",
            2,
            argc,
            argv,
            [] { cout << "A rectangle has 4 sides broken into 2 side pairs of equal length and are parallel.\n"; },
            [](const auto& p) { return 2 * (p[0] + p[1]); },
            [](const auto& p) { return p[0] * p[1]; }
        );
    };
    return true;
}();


//
// ======================================================
// MAIN FUNCTION
// ======================================================
//

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Usage: " << argv[0] << " SHAPE_NAME [PARAMETERS...]\n";    // Basic usage instruction
        return 1;
    }
    auto shape = Shape::create(argc, argv);
    if (!shape) {
        cout<<"Shape Creation Failed\n";   // Exit if shape creation failed
        return 0;
    }
    shape->describe();
    shape->compute();

    return 0;
}

