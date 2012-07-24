#include <cstdlib>
#include <iostream>

#include "Error.h"
#include "Interpolation.h"

interpolation_t interp_data;

void initInterpolation(FieldV field)
{
    interp_data.resize(20);
    switch(field.dim())
    {
        case 2:
            interp_data.resize(2);
            for(int i = 0; i < 2; i++)
            {
                interp_data[i].resize(1);
                interp_data[i].setZero();
            }
            // x term
            interp_data[0] << (field.coords[1]);
            // constant term
            interp_data[1] << (field.coords[0]);
            break;
        case 4:
            interp_data.resize(4);
            for(int i = 0; i < 4; i++)
            {
                interp_data[i].resize(2);
                interp_data[i].setZero();
            }
            // x term
            interp_data[0] << (field.coords[2] - field.coords[0]);
            // y term
            interp_data[1] << (field.coords[1] - field.coords[0]);
            // xy term
            interp_data[2] << (field.coords[3] - field.coords[2] - field.coords[1] + field.coords[0]);
            // constant term;
            interp_data[3] << (field.coords[0]);
            break;
        case 8:
            interp_data.resize(8);
            for(int i = 0; i < 8; i++)
            {
                interp_data[i].resize(3);
                interp_data[i].setZero();
            }
            // x term
            interp_data[0] << (field.coords[4] - field.coords[0]);
            // y term
            interp_data[1] << (field.coords[2] - field.coords[0]);
            // z term
            interp_data[2] << (field.coords[1] - field.coords[0]);
            // x^2y term
            interp_data[3] << (field.coords[6] - field.coords[4] - field.coords[2] + field.coords[0]);
            // x^2z term
            interp_data[4] << (field.coords[5] - field.coords[4] - field.coords[1] + field.coords[0]);
            // y^2z term
            interp_data[5] << (field.coords[3] - field.coords[2] - field.coords[1] + field.coords[0]);
            // xyz term
            interp_data[6] << (field.coords[7] - field.coords[6] - field.coords[5] + field.coords[4] - field.coords[3] + field.coords[2] + field.coords[1] - field.coords[0]);
            // constant term
            interp_data[7] << (field.coords[0]);
            break;
        default:
            cerr << FIELD_DIM_UNSUP_ERR;
            exit(FIELD_DIM_UNSUP);
    }
    return;
}

RowVectorXd interpolate(RowVectorXd coord)
{
    RowVectorXd temp_vec;
    temp_vec.resize(coord.size());

    switch(temp_vec.size())
    {
        case 1:
            temp_vec << interp_data[0] * coord[0] + interp_data[1];
            break;
        case 2:
            temp_vec << interp_data[0] * coord[0] + interp_data[1] * coord[1] + interp_data[2] * coord[0] * coord[1] + interp_data[3];
            break;
        case 3:
            temp_vec << interp_data[0] * coord[0] + interp_data[1] * coord[1] + interp_data[2] * coord[2] + interp_data[3] * coord[0] * coord[1] + interp_data[4] * coord[0] * coord[2] + interp_data[5] * coord[1] * coord[2] + interp_data[6] * coord[0] * coord[1] * coord[2] + interp_data[7];
            break;
        default:
            cerr << FIELD_DIM_UNSUP_ERR;
            exit(FIELD_DIM_UNSUP);
    }

    return temp_vec;
}
