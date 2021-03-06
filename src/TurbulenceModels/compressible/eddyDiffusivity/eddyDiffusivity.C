/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2015 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/

#include "eddyDiffusivity.H"

// * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * //

template<class BasicTurbulenceModel>
void Foam::eddyDiffusivity<BasicTurbulenceModel>::correctNut()
{
    alphat_ = this->rho_*this->nut()/Prt_;
    alphat_.correctBoundaryConditions();
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class BasicTurbulenceModel>
Foam::eddyDiffusivity<BasicTurbulenceModel>::eddyDiffusivity
(
    const word& type,
    const geometricOneField& alpha,
    const volScalarField& rho,
    const volVectorField& U,
    const surfaceScalarField& alphaRhoPhi,
    const surfaceScalarField& phi,
    const transportModel& transport,
    const word& propertiesName
)
:
    BasicTurbulenceModel
    (
        type,
        alpha,
        rho,
        U,
        alphaRhoPhi,
        phi,
        transport,
        propertiesName
    ),

    // Prt_
    // (
    //     dimensioned<scalar>::lookupOrAddToDict
    //     (
    //         "Prt",
    //         this->coeffDict_,
    //         1.0
    //     )
    // ),

    Prt_("Prt", dimless, 1.0),

    alphat_
    (
        IOobject
        (
            "alphat",
            this->runTime_.timeName(),
            this->mesh_,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        this->mesh_
    )
{}


// * * * * * * * * * * * * * * * * * Selectors * * * * * * * * * * * * * * * //

template<class BasicTurbulenceModel>
Foam::autoPtr<Foam::eddyDiffusivity<BasicTurbulenceModel> >
Foam::eddyDiffusivity<BasicTurbulenceModel>::New
(
    const volScalarField& rho,
    const volVectorField& U,
    const surfaceScalarField& phi,
    const transportModel& transport,
    const word& propertiesName
)
{
    return autoPtr<eddyDiffusivity>
    (
        static_cast<eddyDiffusivity*>(
        BasicTurbulenceModel::New
        (
            rho,
            U,
            phi,
            transport,
            propertiesName
        ).ptr())
    );
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class BasicTurbulenceModel>
bool Foam::eddyDiffusivity<BasicTurbulenceModel>::read()
{
    if (BasicTurbulenceModel::read())
    {
        Prt_.readIfPresent(this->coeffDict());

        return true;
    }
    else
    {
        return false;
    }
}


// ************************************************************************* //
