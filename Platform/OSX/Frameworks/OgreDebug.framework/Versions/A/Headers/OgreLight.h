/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2006 Torus Knot Software Ltd
Also see acknowledgements in Readme.html

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.

You may alternatively use this source under the terms of a specific version of
the OGRE Unrestricted License provided you have obtained such a license from
Torus Knot Software Ltd.
-----------------------------------------------------------------------------
*/
#ifndef _LIGHT_H__
#define _LIGHT_H__

#include "OgrePrerequisites.h"

#include "OgreColourValue.h"
#include "OgreVector3.h"
#include "OgreVector4.h"
#include "OgreString.h"
#include "OgreMovableObject.h"
#include "OgrePlaneBoundedVolume.h"
#include "OgreShadowCameraSetup.h"

namespace Ogre {


    /** Representation of a dynamic light source in the scene.
        @remarks
            Lights are added to the scene like any other object. They contain various
            parameters like type, position, attenuation (how light intensity fades with
            distance), colour etc.
        @par
            The defaults when a light is created is pure white diffuse light, with no
            attenuation (does not decrease with distance) and a range of 1000 world units.
        @par
            Lights are created by using the SceneManager::createLight method. They can subsequently be
            added to a SceneNode if required to allow them to move relative to a node in the scene. A light attached
            to a SceneNode is assumed to have a base position of (0,0,0) and a direction of (0,0,1) before modification
            by the SceneNode's own orientation. If not attached to a SceneNode,
            the light's position and direction is as set using setPosition and setDirection.
        @par
            Remember also that dynamic lights rely on modifying the colour of vertices based on the position of
            the light compared to an object's vertex normals. Dynamic lighting will only look good if the
            object being lit has a fair level of tessellation and the normals are properly set. This is particularly
            true for the spotlight which will only look right on highly tessellated models. In the future OGRE may be
            extended for certain scene types so an alternative to the standard dynamic lighting may be used, such
            as dynamic lightmaps.
    */
    class _OgreExport Light : public MovableObject
    {
    public:
        /// Temp tag used for sorting
        Real tempSquareDist;
		/// internal method for calculating current squared distance from some world position
		void _calcTempSquareDist(const Vector3& worldPos);

        /// Defines the type of light
        enum LightTypes
        {
            /// Point light sources give off light equally in all directions, so require only position not direction
            LT_POINT = 0,
            /// Directional lights simulate parallel light beams from a distant source, hence have direction but no position
            LT_DIRECTIONAL = 1,
            /// Spotlights simulate a cone of light from a source so require position and direction, plus extra values for falloff
            LT_SPOTLIGHT = 2
        };

        /** Default constructor (for Python mainly).
        */
        Light();

        /** Normal constructor. Should not be called directly, but rather the SceneManager::createLight method should be used.
        */
        Light(const String& name);

        /** Standard destructor.
        */
        ~Light();

        /** Sets the type of light - see LightTypes for more info.
        */
        void setType(LightTypes type);

        /** Returns the light type.
        */
        LightTypes getType(void) const;

        /** Sets the colour of the diffuse light given off by this source.
            @remarks
                Material objects have ambient, diffuse and specular values which indicate how much of each type of
                light an object reflects. This value denotes the amount and colour of this type of light the light
                exudes into the scene. The actual appearance of objects is a combination of the two.
            @par
                Diffuse light simulates the typical light emanating from light sources and affects the base colour
                of objects together with ambient light.
        */
        void setDiffuseColour(Real red, Real green, Real blue);

        /** Sets the colour of the diffuse light given off by this source.
            @remarks
                Material objects have ambient, diffuse and specular values which indicate how much of each type of
                light an object reflects. This value denotes the amount and colour of this type of light the light
                exudes into the scene. The actual appearance of objects is a combination of the two.
            @par
                Diffuse light simulates the typical light emanating from light sources and affects the base colour
                of objects together with ambient light.
        */
        void setDiffuseColour(const ColourValue& colour);

        /** Returns the colour of the diffuse light given off by this light source (see setDiffuseColour for more info).
        */
        const ColourValue& getDiffuseColour(void) const;

        /** Sets the colour of the specular light given off by this source.
            @remarks
                Material objects have ambient, diffuse and specular values which indicate how much of each type of
                light an object reflects. This value denotes the amount and colour of this type of light the light
                exudes into the scene. The actual appearance of objects is a combination of the two.
            @par
                Specular light affects the appearance of shiny highlights on objects, and is also dependent on the
                'shininess' Material value.
        */
        void setSpecularColour(Real red, Real green, Real blue);

        /** Sets the colour of the specular light given off by this source.
            @remarks
                Material objects have ambient, diffuse and specular values which indicate how much of each type of
                light an object reflects. This value denotes the amount and colour of this type of light the light
                exudes into the scene. The actual appearance of objects is a combination of the two.
            @par
                Specular light affects the appearance of shiny highlights on objects, and is also dependent on the
                'shininess' Material value.
        */
        void setSpecularColour(const ColourValue& colour);

        /** Returns the colour of specular light given off by this light source.
        */
        const ColourValue& getSpecularColour(void) const;

        /** Sets the attenuation parameters of the light source i.e. how it diminishes with distance.
            @remarks
                Lights normally get fainter the further they are away. Also, each light is given a maximum range
                beyond which it cannot affect any objects.
            @par
                Light attenuation is not applicable to directional lights since they have an infinite range and
                constant intensity.
            @par
                This follows a standard attenuation approach - see any good 3D text for the details of what they mean
                since i don't have room here!
            @param
                range The absolute upper range of the light in world units
            @param
                constant The constant factor in the attenuation formula: 1.0 means never attenuate, 0.0 is complete attenuation
            @param
                linear The linear factor in the attenuation formula: 1 means attenuate evenly over the distance
            @param
                quadratic The quadratic factor in the attenuation formula: adds a curvature to the attenuation formula.
        */
        void setAttenuation(Real range, Real constant, Real linear, Real quadratic);

        /** Returns the absolute upper range of the light.
        */
        Real getAttenuationRange(void) const;

        /** Returns the constant factor in the attenuation formula.
        */
        Real getAttenuationConstant(void) const;

        /** Returns the linear factor in the attenuation formula.
        */
        Real getAttenuationLinear(void) const;

        /** Returns the quadric factor in the attenuation formula.
        */
        Real getAttenuationQuadric(void) const;

        /** Sets the position of the light.
            @remarks
                Applicable to point lights and spotlights only.
            @note
                This will be overridden if the light is attached to a SceneNode.
        */
        void setPosition(Real x, Real y, Real z);

        /** Sets the position of the light.
            @remarks
                Applicable to point lights and spotlights only.
            @note
                This will be overridden if the light is attached to a SceneNode.
        */
        void setPosition(const Vector3& vec);

        /** Returns the position of the light.
            @note
                Applicable to point lights and spotlights only.
        */
        const Vector3& getPosition(void) const;

        /** Sets the direction in which a light points.
            @remarks
                Applicable only to the spotlight and directional light types.
            @note
                This will be overridden if the light is attached to a SceneNode.
        */
        void setDirection(Real x, Real y, Real z);

        /** Sets the direction in which a light points.
            @remarks
                Applicable only to the spotlight and directional light types.
            @note
                This will be overridden if the light is attached to a SceneNode.
        */
        void setDirection(const Vector3& vec);

        /** Returns the light's direction.
            @remarks
                Applicable only to the spotlight and directional light types.
        */
        const Vector3& getDirection(void) const;

        /** Sets the range of a spotlight, i.e. the angle of the inner and outer cones and the rate of falloff between them.
            @param
                innerAngle Angle covered by the bright inner cone
                @node
                    The inner cone applicable only to Direct3D, it'll always treat as zero in OpenGL.
            @param
                outerAngle Angle covered by the outer cone
            @param
                falloff The rate of falloff between the inner and outer cones. 1.0 means a linear falloff, less means slower falloff, higher means faster falloff.
        */
        void setSpotlightRange(const Radian& innerAngle, const Radian& outerAngle, Real falloff = 1.0);

        /** Returns the angle covered by the spotlights inner cone.
        */
        const Radian& getSpotlightInnerAngle(void) const;

        /** Returns the angle covered by the spotlights outer cone.
        */
        const Radian& getSpotlightOuterAngle(void) const;

        /** Returns the falloff between the inner and outer cones of the spotlight.
        */
        Real getSpotlightFalloff(void) const;

		/** Sets the angle covered by the spotlights inner cone.
		*/
		void setSpotlightInnerAngle(const Radian& val);

		/** Sets the angle covered by the spotlights outer cone.
		*/
		void setSpotlightOuterAngle(const Radian& val);

		/** Sets the falloff between the inner and outer cones of the spotlight.
		*/
		void setSpotlightFalloff(Real val);

		/** Set a scaling factor to indicate the relative power of a light.
		@remarks
			This factor is only useful in High Dynamic Range (HDR) rendering.
			You can bind it to a shader variable to take it into account,
			@see GpuProgramParameters
		@param power The power rating of this light, default is 1.0.
		*/
		void setPowerScale(Real power);

		/** Set the scaling factor which indicates the relative power of a 
			light.
		*/
		Real getPowerScale(void) const;

        /** Overridden from MovableObject */
        void _notifyAttached(Node* parent, bool isTagPoint = false);

        /** Overridden from MovableObject */
        void _notifyMoved(void);

        /** Overridden from MovableObject */
        const AxisAlignedBox& getBoundingBox(void) const;

        /** Overridden from MovableObject */
        void _updateRenderQueue(RenderQueue* queue);

        /** Overridden from MovableObject */
        const String& getMovableType(void) const;

        /** Retrieves the position of the light including any transform from nodes it is attached to. 
		@param cameraRelativeIfSet If set to true, returns data in camera-relative units if that's been set up (render use)
		*/
        const Vector3& getDerivedPosition(bool cameraRelativeIfSet = false) const;

        /** Retrieves the direction of the light including any transform from nodes it is attached to. */
        const Vector3& getDerivedDirection(void) const;

        /** Overridden from MovableObject.
        @remarks
            Although lights themselves are not 'visible', setting a light to invisible
            means it no longer affects the scene.
        */
        void setVisible(bool visible);

        /** Overridden from MovableObject */
        Real getBoundingRadius(void) const { return 0; /* not visible */ }

		/** Gets the details of this light as a 4D vector.
		@remarks
			Getting details of a light as a 4D vector can be useful for
			doing general calculations between different light types; for
			example the vector can represent both position lights (w=1.0f)
			and directional lights (w=0.0f) and be used in the same 
			calculations.
		@param cameraRelativeIfSet If set to true, returns data in camera-relative units if that's been set up (render use)
		*/
		Vector4 getAs4DVector(bool cameraRelativeIfSet = false) const;

        /** Internal method for calculating the 'near clip volume', which is
            the volume formed between the near clip rectangle of the 
            camera and the light.
        @remarks This volume is a pyramid for a point/spot light and
            a cuboid for a directional light. It can used to detect whether
            an object could be casting a shadow on the viewport. Note that
            the reference returned is to a shared volume which will be 
            reused across calls to this method.
        */
        virtual const PlaneBoundedVolume& _getNearClipVolume(const Camera* const cam) const;

        /** Internal method for calculating the clip volumes outside of the 
            frustum which can be used to determine which objects are casting
            shadow on the frustum as a whole. 
        @remarks Each of the volumes is a pyramid for a point/spot light and
            a cuboid for a directional light. 
        */
        virtual const PlaneBoundedVolumeList& _getFrustumClipVolumes(const Camera* const cam) const;

		/// Override to return specific type flag
		uint32 getTypeFlags(void) const;

		/// @copydoc AnimableObject::createAnimableValue
		AnimableValuePtr createAnimableValue(const String& valueName);

		/** Set this light to use a custom shadow camera when rendering texture shadows.
		@remarks
			This changes the shadow camera setup for just this light,  you can set
			the shadow camera setup globally using SceneManager::setShadowCameraSetup
		@see ShadowCameraSetup
		*/
		void setCustomShadowCameraSetup(const ShadowCameraSetupPtr& customShadowSetup);

		/** Reset the shadow camera setup to the default. 
		@see ShadowCameraSetup
		*/
		void resetCustomShadowCameraSetup(void);

		/** return a pointer to the custom shadow camera setup (null means use SceneManager global version). */
		const ShadowCameraSetupPtr& getCustomShadowCameraSetup(void) const;

		/// @copydoc MovableObject::visitRenderables
		void visitRenderables(Renderable::Visitor* visitor, 
			bool debugRenderables = false);

		/** Gets the index at which this light is in the current render. 
		@remarks
			Lights will be present in the in a list for every renderable,
			detected and sorted appropriately, and sometimes it's useful to know 
			what position in that list a given light occupies. This can vary 
			from frame to frame (and object to object) so you should not use this
			value unless you're sure the context is correct.
		*/
		size_t _getIndexInFrame() const { return mIndexInFrame; }
		void _notifyIndexInFrame(size_t i) { mIndexInFrame = i; }
		
        /** Sets the maximum distance away from the camera that shadows
        by this light will be visible.
        @remarks
        Shadow techniques can be expensive, therefore it is a good idea
        to limit them to being rendered close to the camera if possible,
        and to skip the expense of rendering shadows for distance objects.
        This method allows you to set the distance at which shadows will no
        longer be rendered.
        @note
        Each shadow technique can interpret this subtely differently.
        For example, one technique may use this to eliminate casters,
        another might use it to attenuate the shadows themselves.
        You should tweak this value to suit your chosen shadow technique
        and scene setup.
        */
        void setShadowFarDistance(Real distance);
        /** Tells the light to use the shadow far distance of the SceneManager
        */
        void resetShadowFarDistance(void);
        /** Gets the maximum distance away from the camera that shadows
        by this light will be visible.
        */
        Real getShadowFarDistance(void) const;
        Real getShadowFarDistanceSquared(void) const;

		/// Set the camera which this light should be relative to, for camera-relative rendering
		void _setCameraRelative(Camera* cam);


    protected:
        /// internal method for synchronising with parent node (if any)
        virtual void update(void) const;

		/// @copydoc AnimableObject::getAnimableDictionaryName
		const String& getAnimableDictionaryName(void) const;
		/// @copydoc AnimableObject::initialiseAnimableDictionary
		void initialiseAnimableDictionary(StringVector& vec) const;

		LightTypes mLightType;
        Vector3 mPosition;
        ColourValue mDiffuse;
        ColourValue mSpecular;

        Vector3 mDirection;

        Radian mSpotOuter;
        Radian mSpotInner;
        Real mSpotFalloff;
        Real mRange;
        Real mAttenuationConst;
        Real mAttenuationLinear;
        Real mAttenuationQuad;
		Real mPowerScale;
		size_t mIndexInFrame;
		bool mOwnShadowFarDist;
		Real mShadowFarDist;
		Real mShadowFarDistSquared;


        mutable Vector3 mDerivedPosition;
        mutable Vector3 mDerivedDirection;
		// Slightly hacky but unless we separate observed light render state from main Light...
		mutable Vector3 mDerivedCamRelativePosition;
		mutable bool mDerivedCamRelativeDirty;
		Camera* mCameraToBeRelativeTo;

        /// Shared class-level name for Movable type
        static String msMovableType;

        mutable PlaneBoundedVolume mNearClipVolume;
        mutable PlaneBoundedVolumeList mFrustumClipVolumes;
        /// Is the derived transform dirty?
        mutable bool mDerivedTransformDirty;

		/// Pointer to a custom shadow camera setup
		mutable ShadowCameraSetupPtr mCustomShadowCameraSetup;

    };

	/** Factory object for creating Light instances */
	class _OgreExport LightFactory : public MovableObjectFactory
	{
	protected:
		MovableObject* createInstanceImpl( const String& name, const NameValuePairList* params);
	public:
		LightFactory() {}
		~LightFactory() {}

		static String FACTORY_TYPE_NAME;

		const String& getType(void) const;
		void destroyInstance( MovableObject* obj);  

	};

} // Namespace
#endif
