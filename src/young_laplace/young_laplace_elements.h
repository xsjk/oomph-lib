// LIC// ====================================================================
// LIC// This file forms part of oomph-lib, the object-oriented,
// LIC// multi-physics finite-element library, available
// LIC// at http://www.oomph-lib.org.
// LIC//
// LIC// Copyright (C) 2006-2024 Matthias Heil and Andrew Hazel
// LIC//
// LIC// This library is free software; you can redistribute it and/or
// LIC// modify it under the terms of the GNU Lesser General Public
// LIC// License as published by the Free Software Foundation; either
// LIC// version 2.1 of the License, or (at your option) any later version.
// LIC//
// LIC// This library is distributed in the hope that it will be useful,
// LIC// but WITHOUT ANY WARRANTY; without even the implied warranty of
// LIC// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// LIC// Lesser General Public License for more details.
// LIC//
// LIC// You should have received a copy of the GNU Lesser General Public
// LIC// License along with this library; if not, write to the Free Software
// LIC// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
// LIC// 02110-1301  USA.
// LIC//
// LIC// The authors may be contacted at oomph-lib@maths.man.ac.uk.
// LIC//
// LIC//====================================================================
// Header file for YoungLaplace elements
#ifndef OOMPH_YOUNGLAPLACE_ELEMENTS_HEADER
#define OOMPH_YOUNGLAPLACE_ELEMENTS_HEADER


// Config header generated by autoconfig
#ifdef HAVE_CONFIG_H
#include <oomph-lib-config.h>
#endif


// OOMPH-LIB headers
#include "../generic/nodes.h"
#include "../generic/Qelements.h"


namespace oomph
{
  //=============================================================
  /// A class for all isoparametric elements that solve the
  /// YoungLaplace equations.
  /// \f[ div (\frac{1}{W} \nabla u) = \kappa \f]
  /// with
  /// \f[ W^2=1+\|\nabla u\|^2 \f]
  /// These equations can either be solved in the above (cartesian)
  /// form, or in a parametric representation using the method
  /// of spines. See the theory write-up in the documentation for
  /// details.
  /// This class contains the generic maths. Shape functions, geometric
  /// mapping etc. must get implemented in derived class.
  //=============================================================
  class YoungLaplaceEquations : public virtual FiniteElement
  {
  public:
    /// Function pointer to "spine base" function
    typedef void (*SpineBaseFctPt)(const Vector<double>& x,
                                   Vector<double>& spine_base,
                                   Vector<Vector<double>>& dspine_base);

    /// Function pointer to "spine" function
    typedef void (*SpineFctPt)(const Vector<double>& x,
                               Vector<double>& spine,
                               Vector<Vector<double>>& dspine);

    /// Constructor: Initialise pointers to NULL, so by default
    /// prescribed kappa evaluates to zero, and no spines are used.
    YoungLaplaceEquations() : Spine_base_fct_pt(0), Spine_fct_pt(0), Kappa_pt(0)
    {
    }

    /// Broken copy constructor
    YoungLaplaceEquations(const YoungLaplaceEquations& dummy) = delete;

    /// Broken assignment operator
    void operator=(const YoungLaplaceEquations&) = delete;

    /// Access function: Nodal function value at local node n
    /// Uses suitably interpolated value for hanging nodes.
    virtual inline double u(const unsigned& n) const
    {
      return nodal_value(n, 0);
    }

    /// Output with default number of plot points
    void output(std::ostream& outfile)
    {
      unsigned n_plot = 5;
      output(outfile, n_plot);
    }

    /// Output FE representation of soln
    /// at n_plot^2 plot points
    void output(std::ostream& outfile, const unsigned& n_plot);


    /// Output exact soln at n_plot^2 plot points
    void output_fct(std::ostream& outfile,
                    const unsigned& n_plot,
                    FiniteElement::SteadyExactSolutionFctPt exact_soln_pt);


    /// Output exact soln at
    /// n_plot^2 plot points (dummy time-dependent version to
    /// keep intel compiler happy)
    virtual void output_fct(
      std::ostream& outfile,
      const unsigned& n_plot,
      const double& time,
      FiniteElement::UnsteadyExactSolutionFctPt exact_soln_pt)
    {
      throw OomphLibError("These equations are steady => no time dependence",
                          OOMPH_CURRENT_FUNCTION,
                          OOMPH_EXCEPTION_LOCATION);
    }


    /// Get error against and norm of exact solution
    void compute_error(std::ostream& outfile,
                       FiniteElement::SteadyExactSolutionFctPt exact_soln_pt,
                       double& error,
                       double& norm);

    /// Dummy, time dependent error checker
    void compute_error(std::ostream& outfile,
                       FiniteElement::UnsteadyExactSolutionFctPt exact_soln_pt,
                       const double& time,
                       double& error,
                       double& norm)
    {
      throw OomphLibError("These equations are steady => no time dependence",
                          OOMPH_CURRENT_FUNCTION,
                          OOMPH_EXCEPTION_LOCATION);
    }


    /// Access function: Pointer Data object that stores kappa (const
    /// version -- kappa must be set with set_kappa() which also ensures
    /// that the Data object is added to the element's external Data.
    Data* kappa_pt()
    {
      return Kappa_pt;
    }

    /// Use spines or not? (Based on availability of function pointers
    /// to to spine and spine base vector fields)
    bool use_spines() const
    {
      return (Spine_fct_pt != 0);
    }

    /// Access function to function pointer that specifies spine base
    /// vector field
    SpineBaseFctPt& spine_base_fct_pt()
    {
      return Spine_base_fct_pt;
    }


    /// Access function to function pointer that specifies spine
    /// vector field
    SpineFctPt& spine_fct_pt()
    {
      return Spine_fct_pt;
    }

    /// Set curvature data (and add it to the element's external Data)
    void set_kappa(Data* kappa_pt)
    {
#ifdef PARANOID
      if (kappa_pt->nvalue() != 1)
      {
        throw OomphLibError("kappa must only store a single value!",
                            OOMPH_CURRENT_FUNCTION,
                            OOMPH_EXCEPTION_LOCATION);
      }
#endif

      // Make a local copy
      Kappa_pt = kappa_pt;

      // Add to external data and store index in this storage scheme
      Kappa_index = add_external_data(kappa_pt);
    }


    /// Get curvature
    double get_kappa() const
    {
      /// No kappa has been set: return zero (the default)
      if (Kappa_pt == 0)
      {
        return 0.0;
      }
      else
      {
        // Get prescribed kappa value
        return Kappa_pt->value(0);
      }
    }


    /// Get flux: flux[i] = du/dx_i: Mainly used for error estimation.
    void get_flux(const Vector<double>& s, Vector<double>& flux) const
    {
      // Find out how many nodes there are in the element
      unsigned n_node = nnode();

      // Set up memory for the shape (same as test functions)
      Shape psi(n_node);
      DShape dpsidx(n_node, 2);

      // Call the derivatives of the shape (same as test functions)
      dshape_eulerian(s, psi, dpsidx);

      // Initialise to zero
      for (unsigned j = 0; j < 2; j++)
      {
        flux[j] = 0.0;
      }

      // Loop over nodes
      for (unsigned l = 0; l < n_node; l++)
      {
        // Loop over derivative directions
        for (unsigned j = 0; j < 2; j++)
        {
          flux[j] += u(l) * dpsidx(l, j);
        }
      }
    }


    /// Get spine base vector field: Defaults to standard cartesian
    /// representation if no spine base fct pointers have been set.
    /// dspine_B[i][j] = d spine_B[j] / dx_i
    inline virtual void get_spine_base(
      const Vector<double>& x,
      Vector<double>& spine_base,
      Vector<Vector<double>>& dspine_base) const
    {
      // If no spine function has been set, default to vertical spines
      // emanating from x[0](,x[1])
      if (Spine_base_fct_pt == 0)
      {
        for (unsigned i = 0; i < 3; i++)
        {
          spine_base[i] = x[i];
          for (unsigned j = 0; j < 2; j++)
          {
            dspine_base[i][j] = 0.0;
          }
        }
      }
      else
      {
        // Get spine
        (*Spine_base_fct_pt)(x, spine_base, dspine_base);
      }
    }

    /// Get spine vector field: Defaults to standard cartesian
    /// representation if no spine base fct pointers have been set.
    /// dspine[i][j] = d spine[j] / dx_i
    inline void get_spine(const Vector<double>& x,
                          Vector<double>& spine,
                          Vector<Vector<double>>& dspine) const
    {
      // If no spine function has been set, default to vertical spines
      // emanating from x[0](,x[1])
      if (Spine_fct_pt == 0)
      {
        // Initialise all to zero
        for (unsigned i = 0; i < 3; i++)
        {
          spine[i] = 0.0;
          for (unsigned j = 0; j < 2; j++)
          {
            dspine[i][j] = 0.0;
          }
        }
        // Overwrite vertical component
        spine[2] = 1.0;
      }
      else
      {
        // Get spine
        (*Spine_fct_pt)(x, spine, dspine);
      }
    }

    /// Get position vector to meniscus at local coordinate s
    void position(const Vector<double>& s, Vector<double>& r) const;

    /// Get exact position vector to meniscus at local coordinate s
    void exact_position(const Vector<double>& s,
                        Vector<double>& r,
                        FiniteElement::SteadyExactSolutionFctPt exact_soln_pt);

    /// Add the element's contribution to its residual vector
    void fill_in_contribution_to_residuals(Vector<double>& residuals);


    /// Return FE representation of function value u(s) at local coordinate s
    inline double interpolated_u(const Vector<double>& s) const
    {
      // Find number of nodes
      unsigned n_node = nnode();

      // Local shape function
      Shape psi(n_node);

      // Find values of shape function
      shape(s, psi);

      // Initialise value of u
      double interpolated_u = 0.0;

      // Loop over the local nodes and sum
      for (unsigned l = 0; l < n_node; l++)
      {
        interpolated_u += u(l) * psi[l];
      }

      return (interpolated_u);
    }


    /// Self-test: Return 0 for OK
    unsigned self_test();


    /// Helper fct: Allocate storage for a vector of vectors of doubles
    /// to v(n_rows,n_cols) and initialise each component to 0.0.
    static void allocate_vector_of_vectors(unsigned n_rows,
                                           unsigned n_cols,
                                           Vector<Vector<double>>& v)
    {
      v.resize(n_rows);
      for (unsigned i = 0; i < n_rows; i++)
      {
        v[i].resize(n_cols);
        for (unsigned j = 0; j < n_cols; j++)
        {
          v[i][j] = 0.0;
        }
      }
    }

    /// Multiply a vector by a scalar
    static void scalar_times_vector(const double& lambda,
                                    const Vector<double>& v,
                                    Vector<double>& lambda_times_v)
    {
      unsigned n = v.size();
      for (unsigned i = 0; i < n; i++)
      {
        lambda_times_v[i] = lambda * v[i];
      }
    }


    /// 2-norm of a vector
    static double two_norm(const Vector<double>& v)
    {
      double norm = 0.0;
      unsigned n = v.size();
      for (unsigned i = 0; i < n; i++)
      {
        norm += v[i] * v[i];
      }
      return sqrt(norm);
    }


    /// Scalar product between two vectors
    static double scalar_product(const Vector<double>& v1,
                                 const Vector<double>& v2)
    {
      double scalar = 0.0;
      unsigned n = v1.size();
      for (unsigned i = 0; i < n; i++)
      {
        scalar += v1[i] * v2[i];
      }
      return scalar;
    }

    /// Cross-product: v_cross= v1 x v2
    static void cross_product(const Vector<double>& v1,
                              const Vector<double>& v2,
                              Vector<double>& v_cross)
    {
#ifdef PARANOID
      if ((v1.size() != v2.size()) || (v1.size() != 3))
      {
        throw OomphLibError("Vectors must be of dimension 3 for cross-product!",
                            OOMPH_CURRENT_FUNCTION,
                            OOMPH_EXCEPTION_LOCATION);
      }
#endif
      v_cross[0] = v1[1] * v2[2] - v1[2] * v2[1];
      v_cross[1] = v1[2] * v2[0] - v1[0] * v2[2];
      v_cross[2] = v1[0] * v2[1] - v1[1] * v2[0];
    }

    /// Vectorial sum of two vectors
    static void vector_sum(const Vector<double>& v1,
                           const Vector<double>& v2,
                           Vector<double>& vs)
    {
      unsigned n = v1.size();
      for (unsigned i = 0; i < n; i++)
      {
        vs[i] = v1[i] + v2[i];
      }
    }

  protected:
    /// Get the local equation number of the (one and only) unknown
    /// stored at local node n (returns -1 if value is pinned).
    /// Can be overloaded in derived multiphysics elements.
    virtual inline int u_local_eqn(const unsigned& n)
    {
      return nodal_local_eqn(n, 0);
    }

    /// Index of Kappa_pt in the element's storage of external Data
    unsigned Kappa_index;

    /// Pointer to spine base function:
    SpineBaseFctPt Spine_base_fct_pt;

    /// Pointer to spine function:
    SpineFctPt Spine_fct_pt;

  private:
    /// Pointer to Data item that stores kappa as its first value
    /// -- private to ensure that it must be set with
    /// set_kappa(...) which adds the Data to the element's internal
    /// Data!
    Data* Kappa_pt;
  };


  /// ////////////////////////////////////////////////////////////////////////
  /// ////////////////////////////////////////////////////////////////////////
  /// ////////////////////////////////////////////////////////////////////////


  //======================================================================
  /// QYoungLaplaceElement elements are linear/quadrilateral/brick-shaped
  /// YoungLaplace elements with isoparametric interpolation for the function.
  //======================================================================
  template<unsigned NNODE_1D>
  class QYoungLaplaceElement : public virtual QElement<2, NNODE_1D>,
                               public virtual YoungLaplaceEquations
  {
  private:
    /// Static array of ints to hold number of variables at
    /// nodes: Initial_Nvalue[n]
    static const unsigned Initial_Nvalue[];

  public:
    /// Constructor: Call constructors for QElement and
    /// YoungLaplace equations
    QYoungLaplaceElement() : QElement<2, NNODE_1D>(), YoungLaplaceEquations() {}

    /// Broken copy constructor
    QYoungLaplaceElement(const QYoungLaplaceElement<NNODE_1D>& dummy) = delete;

    /// Broken assignment operator
    void operator=(const QYoungLaplaceElement<NNODE_1D>&) = delete;

    ///  Required  # of `values' (pinned or dofs)
    /// at node n
    inline unsigned required_nvalue(const unsigned& n) const
    {
      return Initial_Nvalue[n];
    }

    /// Output function
    void output(std::ostream& outfile)
    {
      YoungLaplaceEquations::output(outfile);
    }


    ///  Output function at n_plot^2 plot points
    void output(std::ostream& outfile, const unsigned& n_plot)
    {
      YoungLaplaceEquations::output(outfile, n_plot);
    }


    /// Output function for an exact solutio at n_plot^2 plot points
    void output_fct(std::ostream& outfile,
                    const unsigned& n_plot,
                    FiniteElement::SteadyExactSolutionFctPt exact_soln_pt)
    {
      YoungLaplaceEquations::output_fct(outfile, n_plot, exact_soln_pt);
    }


    /// Output function for a time-dependent exact solution
    /// at n_plot^2 plot points (calls the steady version)
    void output_fct(std::ostream& outfile,
                    const unsigned& n_plot,
                    const double& time,
                    FiniteElement::UnsteadyExactSolutionFctPt exact_soln_pt)
    {
      YoungLaplaceEquations::output_fct(outfile, n_plot, time, exact_soln_pt);
    }
  };


  /// /////////////////////////////////////////////////////////////////////
  /// /////////////////////////////////////////////////////////////////////
  /// /////////////////////////////////////////////////////////////////////


  //=======================================================================
  /// Face geometry for the QYoungLaplaceElement elements: The spatial
  /// dimension of the face elements is one lower than that of the
  /// bulk element but they have the same number of points
  /// along their 1D edges.
  //=======================================================================
  template<unsigned NNODE_1D>
  class FaceGeometry<QYoungLaplaceElement<NNODE_1D>>
    : public virtual QElement<1, NNODE_1D>
  {
  public:
    /// Constructor: Call the constructor for the
    /// appropriate lower-dimensional QElement
    FaceGeometry() : QElement<1, NNODE_1D>() {}
  };


  /// /////////////////////////////////////////////////////////////////////
  /// /////////////////////////////////////////////////////////////////////
  /// /////////////////////////////////////////////////////////////////////


  //========================================================================
  /// Height control element for YoungLaplace equations: Prescribe
  /// displacement along a spine (i.e. the "height of the meniscus"
  /// in exchange for treating the curvature as an unknown. Very
  /// similar to the DisplacementControlElement used in solid
  /// mechanics problems.
  //========================================================================
  class HeightControlElement : public virtual GeneralisedElement
  {
  public:
    /// Constructor: Pass pointer to node at which the height is controlled
    /// and pointer to double that stores the prescribed height at that
    /// node.
    HeightControlElement(Node* control_node_pt, double* prescribed_height_pt)
      : GeneralisedElement()
    {
      // Store pointer to prescribed height value
      Prescribed_height_pt = prescribed_height_pt;

      // Store pointer to Node at which the height is controlled
      Control_node_pt = control_node_pt;

      // Create curvature Data, add it as internal data, and store
      // its number with the internal data storage scheme
      // (i.e. internal_data_pt(Curvature_data_index) will return
      // the pointer to it...).
      Curvature_data_index = add_internal_data(new Data(1));

      // Add control_node_pt as external data
      add_external_data(static_cast<Data*>(control_node_pt));
    }

    /// Access function to the pointer to the Data object that
    /// stores the curvature.
    Data*& kappa_pt()
    {
      return internal_data_pt(Curvature_data_index);
    }

    /// Setup local equation number for the height-control equation
    void assign_additional_local_eqn_numbers()
    {
      // Get equation number from generic scheme: The height control
      // equation is "the equation for the curvature" which is
      // stored as internal data in the element and has been
      // numbered as such...
      Height_ctrl_local_eqn = internal_local_eqn(Curvature_data_index, 0);
    }


    /// Add the element's contribution to its residual vector:
    /// The height constraint. [Note: Jacobian is computed
    /// automatically by finite-differencing]
    void fill_in_contribution_to_residuals(Vector<double>& residuals)
    {
      residuals[Height_ctrl_local_eqn] =
        Control_node_pt->value(0) - (*Prescribed_height_pt);
    }

  private:
    /// Pointer to value that stores the controlled height
    double* Prescribed_height_pt;

    /// Pointer to node at which the height is controlled
    Node* Control_node_pt;

    /// In which component (in the vector of the element's internal
    /// Data) is the unknown curvature stored?
    unsigned Curvature_data_index;

    /// Local equation number of the height-control equation
    unsigned Height_ctrl_local_eqn;
  };


} // namespace oomph


#endif
