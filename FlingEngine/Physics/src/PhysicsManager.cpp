#include "Physics/inc/PhysicsManager.h"
#include "Gameplay/inc/Components/Transform.h"

namespace Fling
{
    void PhysicsManager::Init(entt::registry* t_Registry)
    {
        m_Registry = t_Registry;
        //Intialize bullet physics
        m_CollisionConfiguration = new btDefaultCollisionConfiguration();
        m_CollisionDispatcher = new btCollisionDispatcher(m_CollisionConfiguration);
        m_OverlappingPairCache = new btDbvtBroadphase();
        m_Solver = new btSequentialImpulseConstraintSolver();
        m_DynamicsWorld = new btDiscreteDynamicsWorld(
            m_CollisionDispatcher, 
            m_OverlappingPairCache, 
            m_Solver,
            m_CollisionConfiguration);

        m_DynamicsWorld->setGravity(m_Gravity);

        InitComponentData();
    }

    void PhysicsManager::Shutdown()
    {
        //TODO:: Remove rigidbodies 

        //TODO:: Remove contact bodies 

        //TODO:: Delete usepointers from all bodies


        delete m_DynamicsWorld;
        delete m_Solver;
        delete m_OverlappingPairCache;
        delete m_CollisionDispatcher;
        delete m_CollisionConfiguration;
    }

    void PhysicsManager::Tick(float DeltaTime)
    {
        //Tick subsystems
    }

    void PhysicsManager::Update(entt::registry& t_Reg, float DeltaTime)
    {
        m_DynamicsWorld->stepSimulation(DeltaTime);

        //Update registrys
        for (int i = m_DynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
        {
            btTransform trans;
            //auto a = m_DynamicsWorld->getCollisionWorld()->getCollisionObjectArray();
            btCollisionObject* obj = m_DynamicsWorld->getCollisionObjectArray()[i];
            btRigidBody* body = btRigidBody::upcast(obj);
            auto s = body->getLocalInertia();
            entt::entity& entity = static_cast<PhysicsEntity*>(body->getUserPointer())->m_Entity;
            Transform& transform = t_Reg.get<Transform>(entity);

            if (body && body->getMotionState())
            {
                body->getMotionState()->getWorldTransform(trans);
            }
            else
            {
                trans = obj->getWorldTransform();
            }

            //float yaw, pitch, roll;
            transform.m_QRotation =  MathConversions::bulletToGlm(trans.getRotation());
            //auto s = body->getAngularVelocity();
            //F_LOG_TRACE("ROTATION: {:05.2f} {:03.2f} {:03.2f}", yaw, pitch, roll);

            glm::vec3 position = glm::vec3(MathConversions::bulletToGlm(trans.getOrigin()));

            //transform.SetRotation(glm::vec3(roll, pitch, yaw));
            transform.SetPos(position);
        }
    }

    void PhysicsManager::SetGravity(const btVector3& t_Gravity)
    {
        assert(m_DynamicsWorld);
        m_Gravity = t_Gravity;
        m_DynamicsWorld->setGravity(m_Gravity);
    }

    void PhysicsManager::InitComponentData()
    {
        m_Registry->on_construct<Components::Rigidbody>().connect<&PhysicsManager::RigidBodyAdded>(*this);
        m_Registry->on_destroy<Components::Rigidbody>().connect<&PhysicsManager::RigidBodyRemoved>(*this);
        m_Registry->on_replace<Components::Rigidbody>().connect<&PhysicsManager::RigidBodyReplaced>(*this);
    }

    void PhysicsManager::RigidBodyAdded(
        entt::entity t_Ent,
        entt::registry& t_Reg, 
        Components::Rigidbody& t_Rigidbody)
    {
        if (!t_Rigidbody.m_Collider)
        {
            F_LOG_ERROR("Collider component needs to be attached when using rigidbody");
            assert(t_Rigidbody.m_Collider);
        }

        bool hasTransform = t_Reg.has<Transform>(t_Ent);
        if (!hasTransform)
        {
            F_LOG_ERROR("Transform component needs to be attached when using rigidbody");
            assert(hasTransform);
        }

        Transform& transform = t_Reg.get<Transform>(t_Ent);
        //btTransform worldTransform = MathConversions::glmToBullet(transform.GetWorldMatrix());
        btTransform worldTransform = MathConversions::glmToBullet(transform.GetPos(), transform.GetRotation());

        btDefaultMotionState* myMotionState = new btDefaultMotionState(worldTransform);
        
        btVector3 localInertia = { 0.0f, 0.0f, 0.0f };
        if(t_Rigidbody.m_Mass)
            t_Rigidbody.m_Collider->calculateLocalInertia(t_Rigidbody.m_Mass, localInertia);

        btRigidBody::btRigidBodyConstructionInfo rbInfo(
            t_Rigidbody.m_Mass, 
            myMotionState, 
            t_Rigidbody.m_Collider.get(),
            localInertia);

        t_Rigidbody.m_Rigidbody = std::make_unique<btRigidBody>(rbInfo);
        
        PhysicsEntity* physicsEntity = new PhysicsEntity(t_Ent);
        t_Rigidbody.m_Rigidbody->setUserPointer(static_cast<void*>(physicsEntity));

        //rigidbody properties
        t_Rigidbody.m_Rigidbody->setWorldTransform(worldTransform);
        t_Rigidbody.m_Rigidbody->setFriction(t_Rigidbody.m_Friction);
        t_Rigidbody.m_Rigidbody->setRollingFriction(t_Rigidbody.m_FrictionRolling);
        t_Rigidbody.m_Rigidbody->setSpinningFriction(t_Rigidbody.m_FrictionSpinning);
        t_Rigidbody.m_Rigidbody->setGravity(m_Gravity);
        t_Rigidbody.m_Rigidbody->setLinearFactor(t_Rigidbody.m_LinearFactor);
        t_Rigidbody.m_Rigidbody->setAngularFactor(t_Rigidbody.m_AngularFactor);
        //add the body to the dynamics world
        m_DynamicsWorld->addRigidBody(t_Rigidbody.m_Rigidbody.get());
        t_Rigidbody.RecalculateMass();
    }

    void PhysicsManager::RigidBodyRemoved(
        entt::entity t_Ent,
        entt::registry& t_Reg)
    {
    }

    void PhysicsManager::RigidBodyReplaced(
        entt::entity t_Ent, 
        entt::registry& t_Reg, 
        Components::Rigidbody& t_Rigidbody)
    {
    }
}