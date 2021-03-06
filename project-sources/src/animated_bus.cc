/*
 * Copyright (C) 2012 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/
#include <boost/bind.hpp>
#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/common/common.hh>
#include <ignition/math.hh>
#include <stdio.h>
#include <sae_globals.hh>
#include <iostream>
#include <fstream>

namespace gazebo
{
  class AnimatedBus : public ModelPlugin {
    public:

      void Load(physics::ModelPtr _parent, sdf::ElementPtr /*_sdf*/) {

        _model = _parent;

        arrete = true;
        number_passenger = 20;
        hasAnimation = false;
        this->_updateConnection = event::Events::ConnectWorldUpdateBegin(
          boost::bind(&AnimatedBus::OnUpdate, this, _1));

        
      }

    public:
      void OnUpdate(const common::UpdateInfo & /*_info*/) {

        if (!hasAnimation)
        {
          hasAnimation = true;
          loaded = true;
          double Zpos(_model->GetWorldPose().pos.z);
          std::vector<double> x;
          std::vector<double> y;

          for (int i = 0; i < 10; ++i)
          {
            x.push_back(math::Rand::GetDblUniform(-1.0, 1.0));
            y.push_back(math::Rand::GetDblUniform(-1.0, 1.0));
          }

            //Create the animation of the bus
          gazebo::common::PoseAnimationPtr anim(
            new gazebo::common::PoseAnimation("Bus", 260.59, false));

          gazebo::common::PoseKeyFrame *key;

          // set starting location of the bus
          key = anim->CreateKeyFrame(0);
          key->Translation(ignition::math::Vector3d(46.97 + x[0], -31.42 + y[0], Zpos));
          key->Rotation(ignition::math::Quaterniond(0, 0, -1.814));

          key = anim->CreateKeyFrame(14.56677);
          key->Translation(ignition::math::Vector3d(11.67 + x[1], -22.47 + y[1], Zpos));
          key->Rotation(ignition::math::Quaterniond(0, 0, -1.814));

          key = anim->CreateKeyFrame(20.1888);
          key->Translation(ignition::math::Vector3d(4.924 + x[2], -22.548 + y[2], Zpos));
          key->Rotation(ignition::math::Quaterniond(0, 0, -1.371));

          key = anim->CreateKeyFrame(26.805);
          key->Translation(ignition::math::Vector3d(8.674 + x[3], -15.55 + y[3], Zpos));
          key->Rotation(ignition::math::Quaterniond(0, 0, -0.09));

      
          arret = 35.7135;
          depart = arret + 150;
      
          for(auto i = arret; i <= depart; i+=5)
          {
            key = anim->CreateKeyFrame(i);
            key->Translation(ignition::math::Vector3d(9.66879, -4.90602, Zpos));
            key->Rotation(ignition::math::Quaterniond(0, -0, 0));
          }
	
	  key = anim->CreateKeyFrame(190.7135);
          key->Translation(ignition::math::Vector3d(9.66879, -4.90602, Zpos));
          key->Rotation(ignition::math::Quaterniond(0, -0, 0));

          key = anim->CreateKeyFrame(195.7135);
          key->Translation(ignition::math::Vector3d(9.66879 + x[4], -7.6795 + y[4], Zpos));
          key->Rotation(ignition::math::Quaterniond(0, 0, 0));

          key = anim->CreateKeyFrame(201.8135);
          key->Translation(ignition::math::Vector3d(9.66879 + x[5], -15 + y[5], Zpos));
          key->Rotation(ignition::math::Quaterniond(0, 0, 0));

          key = anim->CreateKeyFrame(203.44);
          key->Translation(ignition::math::Vector3d(9.66879 + x[6], -18 + y[6], Zpos));
          key->Rotation(ignition::math::Quaterniond(0, 0, 0.75));

          key = anim->CreateKeyFrame(210.2299);
          key->Translation(ignition::math::Vector3d(20 + x[7], -30 + y[7], Zpos));
          key->Rotation(ignition::math::Quaterniond(0, 0, 0.75));

          key = anim->CreateKeyFrame(213.05849);
          key->Translation(ignition::math::Vector3d(25 + x[8], -35 + y[8], Zpos));
          key->Rotation(ignition::math::Quaterniond(0, 0, 1.57));

          key = anim->CreateKeyFrame(223.05849);
          key->Translation(ignition::math::Vector3d(50 + x[9], -35 + y[9], Zpos));
          key->Rotation(ignition::math::Quaterniond(0, 0, 1.57));

          // set the animation to the model
          _model->SetAnimation(anim);
          _anim = anim;
          _world = _model->GetWorld();
          intervalle = (depart - arret) / double(number_passenger);

          this->_updateConnection = event::Events::ConnectWorldUpdateBegin(
            boost::bind(&AnimatedBus::OnUpdate, this, _1));

        }
        else
        {
          temps = _anim->GetTime();

          if (loaded && temps > arret && temps < depart) {
            const math::Pose &modelPose = _model->GetWorldPose();

            sdf::SDF passengerSDF;
            std::stringstream passengerDesc;
            passengerDesc <<
  			"<sdf version ='1.4'>\
                           <model name ='passenger'>\
                             <pose> 4.07524 0.063808 0.112085 1e-06 -1e-06 1.513</pose>\
                             <link name ='link'>\
                               <pose>0 0 0.2 0 0 0</pose>\
                               <collision name ='collision'>\
                                 <geometry>\
  								<mesh>\
  				 					<scale>1 1 1</scale>\
                                   <uri>model://person_walking/meshes/walking.dae</uri>\
  								</mesh>\
                                 </geometry>\
                               </collision>\
                               <visual name ='visual'>\
                                 <geometry>\
                                 	<mesh>\
  				 					<scale>1 1 1</scale>\
                                   <uri>model://person_walking/meshes/walking.dae</uri>\
  								</mesh>\
                                 </geometry>\
                               </visual>\
                             </link>\
                             <plugin name='animated_passenger' filename='libanimated_passenger.so'/>\
                           </model>\
                         </sdf>";
            passengerSDF.SetFromString(passengerDesc.str());
            // Demonstrate using a custom model name.
            sdf::ElementPtr model = passengerSDF.Root()->GetElement("model");
            model->GetAttribute("name")->SetFromString("unique_passenger");
            _world->InsertModelSDF(passengerSDF);
  	  //_world->InsertModelFile("model://passenger");
            number_passenger--;
  	        arret += intervalle;
            loaded = number_passenger != 0;
          }
          else if (temps > 223.05849)
          {
            _model->StopAnimation();
            loaded = true;
            arrete = true;
            number_passenger = 20;
            hasAnimation = false;
          }
        }

      }

    private:
      physics::ModelPtr _model;
      physics::WorldPtr _world;
      gazebo::common::PoseAnimationPtr _anim;
      event::ConnectionPtr _updateConnection;
      double arret, depart, intervalle;
      double temps;
      int number_passenger = 20;
      bool loaded = false, arrete = true;
      double x, y;
      bool hasAnimation;
  };
  GZ_REGISTER_MODEL_PLUGIN(AnimatedBus);
}
