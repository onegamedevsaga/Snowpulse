#include "spine_skeleton_animation.h"

#include <iostream>
#include <map>

#include <spine/Extension.h>
#include <spine/SkeletonJson.h>
#include <spine/Skin.h>
#include <spine/Bone.h>
#include <spine/BoneData.h>
#include <spine/SlotData.h>
#include <spine/Attachment.h>
#include <spine/RegionAttachment.h>
#include <spine/MeshAttachment.h>
#include <spine/VertexAttachment.h>

#include "spine_texture_loader.h"

spine::SpineExtension *spine::getDefaultExtension() {
    return new spine::DefaultSpineExtension();
}

namespace snowpulse {
std::shared_ptr<SpineSkeletonAnimation> SpineSkeletonAnimation::Create(std::string jsonFilename, std::string atlasFilename, TextureFiltering filtering) {
    auto spineSkeletonAnimation = std::shared_ptr<SpineSkeletonAnimation>(new SpineSkeletonAnimation());
    spineSkeletonAnimation->textureFiltering_ = filtering;
    spineSkeletonAnimation->textureLoader_ = SpineTextureLoader::Create(spineSkeletonAnimation->textureFiltering_);
    spineSkeletonAnimation->atlas_ = std::make_shared<spine::Atlas>(atlasFilename.c_str(), spineSkeletonAnimation->textureLoader_.get());
    if (spineSkeletonAnimation->atlas_->getPages().size() == 0) {
#ifdef SPDEBUG
    std::cerr << "Error: Failed to load spine atlas (" << atlasFilename << ")." << std::endl;
#endif
        return SPNULL;
    }

    spine::SkeletonJson json(spineSkeletonAnimation->atlas_.get());
    spineSkeletonAnimation->skeletonData_ = std::shared_ptr<spine::SkeletonData>(json.readSkeletonDataFile(jsonFilename.c_str()));
    if (!spineSkeletonAnimation->skeletonData_) {
#ifdef SPDEBUG
    std::cerr << "Error: Failed to load spine json (" << jsonFilename << ")." << std::endl;
#endif
        return SPNULL;
    }

    spineSkeletonAnimation->animationStateData_ = std::make_shared<spine::AnimationStateData>(spineSkeletonAnimation->skeletonData_.get());
    //spineSkeletonAnimation->animationStateData_->
    //spineSkeletonAnimation->animationStateData_->setDefaultMix(0.5f);
    //spineSkeletonAnimation->animationStateData_->setMix("walk", "run", 0.2f);
    //spineSkeletonAnimation->animationStateData_->setMix("walk", "shot", 0.1f);
    
    spineSkeletonAnimation->skeleton_ = std::make_shared<spine::Skeleton>(spineSkeletonAnimation->skeletonData_.get());
    //std::string skinName = "full-skins/girl-blue-cape";
    spineSkeletonAnimation->SetSkin("default");
    //spineSkeletonAnimation->SetSkin("full-skins/girl-blue-cape");
    
    spineSkeletonAnimation->animationState_ = std::make_shared<spine::AnimationState>(spineSkeletonAnimation->animationStateData_.get());
    //spineSkeletonAnimation->animationState_->setAnimation(0, "walk", true);

    size_t pos = atlasFilename.find_last_of("/\\");
    spineSkeletonAnimation->jsonFilename_ = jsonFilename;
    spineSkeletonAnimation->atlasPath_ = (pos != std::string::npos) ? atlasFilename.substr(0, pos + 1) : "";
    return spineSkeletonAnimation;
}

SpineSkeletonAnimation::SpineSkeletonAnimation() {
}

SpineSkeletonAnimation::~SpineSkeletonAnimation() {
}

// From Updatable
void SpineSkeletonAnimation::Update(float deltaTime) {
    animationState_->update(deltaTime);
    animationState_->apply(*skeleton_);
    skeleton_->updateWorldTransform();
}

// From Drawable
void SpineSkeletonAnimation::Draw(Graphics* graphics, Matrix4x4 worldMatrix, int sortOrder, bool isPremultiplied) {
    for (int i = 0, n = (int)skeleton_->getSlots().size(); i < n; i++) {
        auto slot = skeleton_->getDrawOrder()[i];
        if (!slotsToDraw_.count(slot)) {
            continue;
        }

        spine::Attachment* attachment = slot->getAttachment();
        if (!attachment) {
            continue;
        }

        BlendMode blendMode;
        switch (slot->getData().getBlendMode()) {
            case spine::BlendMode_Normal:
                blendMode = BlendMode::kNormal;
                break;
            case spine::BlendMode_Additive:
                blendMode = BlendMode::kAdditive;
                break;
            case spine::BlendMode_Multiply:
                blendMode = BlendMode::kMultiply;
                break;
            case spine::BlendMode_Screen:
                blendMode = BlendMode::kScreen;
                break;
            default:
                blendMode = BlendMode::kNormal;
        }

        spine::Color skeletonColor = skeleton_->getColor();
        spine::Color slotColor = slot->getColor();
        spine::Color tint(skeletonColor.r * slotColor.r, skeletonColor.g * slotColor.g, skeletonColor.b * slotColor.b, skeletonColor.a * slotColor.a);

        if (attachment->getRTTI().isExactly(spine::RegionAttachment::rtti)) {
            // Cast to an spRegionAttachment so we can get the rendererObject
            // and compute the world vertices
            auto regionAttachment = (spine::RegionAttachment*)attachment;
            auto attachmentColor = regionAttachment->getColor();

            // Ensure there is enough room for vertices
            unsigned short indices[] = { 0, 1, 2, 2, 3, 0 };
            Vertex vertices[4];
            spine::Vector<float> vertexList;
            vertexList.setSize(4 * 2, 0.0f);

            // Computed the world vertices positions for the 4 vertices that make up
            // the rectangular region attachment. This assumes the world transform of the
            // bone to which the slot (and hence attachment) is attached has been calculated
            // before rendering via Skeleton::updateWorldTransform(). The vertex positions
            // will be written directoy into the vertices array, with a stride of sizeof(Vertex)
            regionAttachment->computeWorldVertices(*slot, vertexList, 0);

            // Our engine specific Texture is stored in the AtlasRegion which was
            // assigned to the attachment on load. It represents the texture atlas
            // page that contains the image the region attachment is mapped to.
            auto textureFilename = *(std::string*)regionAttachment->getRegion()->rendererObject;

            // copy color and UVs to the vertices
            for (size_t j = 0, l = 0; j < 4; j++, l+=2) {
                Vertex& vertex = vertices[j];
                vertex.position.x = vertexList[j * 2 + 0];
                vertex.position.y = vertexList[j * 2 + 1];
                vertex.position.z = 0.0f;
                vertex.color.r = tint.r * attachmentColor.r;
                vertex.color.g = tint.g * attachmentColor.g;
                vertex.color.b = tint.b * attachmentColor.b;
                vertex.color.a = tint.a * attachmentColor.a;
                vertex.uv.x = regionAttachment->getUVs()[l];
                vertex.uv.y = regionAttachment->getUVs()[l + 1];
            }

            graphics->DrawMesh(vertices, 4, indices, 6, graphics->GetTextureFullFilename(textureFilename, textureFiltering_), sortOrder, blendMode, isPremultiplied, worldMatrix);
        }
        else if (attachment->getRTTI().isExactly(spine::MeshAttachment::rtti)) {
            // Cast to an MeshAttachment so we can get the rendererObject
            // and compute the world vertices
            auto mesh = (spine::MeshAttachment*)attachment;
            auto attachmentColor = mesh->getColor();
            spine::Vector<unsigned short>& indices = mesh->getTriangles();

            // Ensure there is enough room for vertices
            size_t numVertices = mesh->getWorldVerticesLength() / 2;
            Vertex vertices[numVertices];
            float vertexList[mesh->getWorldVerticesLength()];
            //vertexList.setSize(mesh->getWorldVerticesLength(), 0.0f);

            // Computed the world vertices positions for the vertices that make up
            // the mesh attachment. This assumes the world transform of the
            // bone to which the slot (and hence attachment) is attached has been calculated
            // before rendering via Skeleton::updateWorldTransform(). The vertex positions will
            // be written directly into the vertices array, with a stride of sizeof(Vertex)
            mesh->computeWorldVertices(*slot, 0, numVertices * 2, vertexList, 0);

            // Our engine specific Texture is stored in the AtlasRegion which was
            // assigned to the attachment on load. It represents the texture atlas
            // page that contains the image the region attachment is mapped to.
            auto textureFilename = *(std::string*)mesh->getRegion()->rendererObject;

            // Copy color and UVs to the vertices
            for (size_t j = 0, l = 0; j < numVertices; j++, l+=2) {
                Vertex& vertex = vertices[j];
                vertex.position.x = vertexList[j * 2 + 0];
                vertex.position.y = vertexList[j * 2 + 1];
                vertex.position.z = 0.0f;
                vertex.color.r = tint.r * attachmentColor.r;
                vertex.color.g = tint.g * attachmentColor.g;
                vertex.color.b = tint.b * attachmentColor.b;
                vertex.color.a = tint.a * attachmentColor.a;
                vertex.uv.x = mesh->getUVs()[l];
                vertex.uv.y = mesh->getUVs()[l + 1];
            }

            graphics->DrawMesh(vertices, (int)numVertices, indices.buffer(), (int)indices.size(), graphics->GetTextureFullFilename(textureFilename, textureFiltering_), sortOrder, blendMode, isPremultiplied, worldMatrix);
        }
        else {
            int xx= 0;
            xx++;
        }
    }
}

void SpineSkeletonAnimation::SetSkin(std::string name) {
    if (skeletonData_->findSkin(name.c_str()) != SPNULL) {
        skeleton_->setSkin(name.c_str());
        skeleton_->setSlotsToSetupPose();
        skeleton_->updateWorldTransform();
    }
    else {
        std::cerr << "Skin (" << name << ") not found on " << jsonFilename_ << "." << std::endl;
        return;
    }
    
    slotsToDraw_.clear();
    std::map<spine::Slot*, bool> nonDefaultSkinSlots;
    auto allSlots = skeleton_->getSlots();
    auto allSkins = skeletonData_->getSkins();
    auto defaultSkin = skeletonData_->getDefaultSkin();
    for (int i = 0, n = (int)allSkins.size(); i < n; i++) {
        auto skin = allSkins[i];
        if (skin != defaultSkin) {
            auto entries = skin->getAttachments();
            while(entries.hasNext()) {
                auto attachment = entries.next();
                auto slot = allSlots[attachment._slotIndex];
                nonDefaultSkinSlots[slot] = true;
            }
            auto bones = skin->getBones();
            auto slots = GetSlotsWithBoneDatas(bones);
            for (auto& slot : slots) {
                nonDefaultSkinSlots[slot] = true;
            }
        }
    }
    auto defaultEntries = defaultSkin->getAttachments();
    while(defaultEntries.hasNext()) {
        auto attachment = defaultEntries.next();
        auto slot = allSlots[attachment._slotIndex];
        if (!nonDefaultSkinSlots.count(slot)) {
            slotsToDraw_[slot] = true;
        }
    }

    

    auto activeSkin = skeleton_->getSkin();
    if (activeSkin != defaultSkin) {
        auto entries = activeSkin->getAttachments();
        while(entries.hasNext()) {
            auto attachment = entries.next();
            auto slot = allSlots[attachment._slotIndex];
            slotsToDraw_[slot] = true;
        }
        auto bones = activeSkin->getBones();
        auto slots = GetSlotsWithBoneDatas(bones);
        for (auto& slot : slots) {
            auto slotParents = GetSlotParents(slot);
            slotsToDraw_[slot] = true;
            for (auto& p : slotParents) {
                slotsToDraw_[p] = true;
            }
        }
    }
}

void SpineSkeletonAnimation::PlayAnimation(std::string name, bool looping) {
    if (!skeletonData_->findAnimation(name.c_str())) {
#ifdef SPDEBUG
    std::cerr << "Error: there's no animation called \"" << name << "\" in " << jsonFilename_ << std::endl;
#endif
        return;
    }
    animationState_->setAnimation(0, name.c_str(), looping);
}

std::vector<spine::Slot*> SpineSkeletonAnimation::GetSlotParents(spine::Slot* slot) {
    std::vector<spine::Slot*> parents;
    auto bone = &slot->getBone();
    while (bone) {
        auto allSlots = skeleton_->getSlots();
        for (int i = 0, n = (int)allSlots.size(); i < n; i++) {
            auto slot = allSlots[i];
            if (slot->getBone().getData().getName() == bone->getData().getName()) {
                parents.push_back(slot);
            }
        }
        bone = bone->getParent();
    }
    return parents;
}

std::vector<spine::Slot*> SpineSkeletonAnimation::GetSlotsWithBoneDatas(spine::Vector<spine::BoneData*> bones) {
    std::vector<spine::Slot*> ret;
    auto allSlots = skeleton_->getSlots();
    for (int i = 0, n = (int)allSlots.size(); i < n; i++) {
        auto slot = allSlots[i];
        if (IsSlotOnBones(slot, bones)) {
            ret.push_back(slot);
        }
    }
    return ret;
}

bool SpineSkeletonAnimation::IsSlotOnBones(spine::Slot* slot, spine::Vector<spine::BoneData*>& bones) {
    for (int i = 0, n = (int)bones.size(); i < n; i++) {
        auto bone = bones[i];
        if (slot->getBone().getData().getName() == bone->getName()) {
            return true;
        }
    }
    return false;
}
}   // namespace snowpulse
