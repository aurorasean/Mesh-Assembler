import unreal
import debugpy_unreal
actor = None


class Runner:
    def __init__(
        self, anims, meshes, textures, skels, asset_ref: unreal.Blueprint
    ):
        self.meshes = meshes
        self.skels = skels
        self.asset_ref = asset_ref

    def enableNanite(self, staticMeshActor):
        """
        Enable Nanite on static mesh associated with static mesh actor.
        """
        staticMesh = staticMeshActor
        if staticMesh:
            # get mesh nanite settings
            meshNaniteSettings = staticMesh.get_editor_property('nanite_settings')
            if not meshNaniteSettings.enabled:
                meshNaniteSettings.enabled = True
                unreal.StaticMeshEditorSubsystem().set_nanite_settings(staticMesh,meshNaniteSettings, apply_changes=True)
                
    def run(self):
        debugpy_unreal.breakpoint()
        print("running blueprint creator: %s" % self.asset_ref.get_path_name())
        # need to update the asset_ref.skeletal mesh to new mesh
        blueprint_asset = unreal.load_asset(self.asset_ref.get_path_name())
        actor1 = unreal.load_asset("%s_C" % (self.asset_ref.get_path_name()))

        actor = unreal.EditorLevelLibrary.spawn_actor_from_class(
            actor1,
            (0, 0, 0),
        )
        actor.set_actor_label("edit_place_holder")
        actor.tags = ["edit_place_holder"]
        sean_utils = unreal.SeanUtils()


        skelCP = actor.get_components_by_tag(tag="Skeleton")[0]
        skelCP.set_skeletal_mesh_asset(self.skels[0])
        
        # hide the skeletal mesh, as it's just flats
        skelCP.set_visibility(False)
        unreal.SeanUtils.apply_instance_changes_to_blueprint(actor)

        sean_utils = unreal.SeanUtils()
        component = unreal.StaticMeshComponent

        sean_utils.get_component_by_name_or_default(blueprint_asset, "")
        blueprint_asset
        for x in range(len(self.meshes)):
            mesh = self.meshes[x]
            mesh_name = mesh.get_name()
            existing = sean_utils.get_component_by_name_or_default(
                blueprint_asset, mesh_name
            )
            if existing and existing.get_name() == "%s_GEN_VARIABLE" % mesh_name:
                continue

            newStaticComp = sean_utils.add_component_to_actor(
                blueprint_asset, component, mesh_name, "SkeletalMesh"
            )
            newStaticComp.set_static_mesh(mesh)
            
            sean_utils.attach_to_comp(
                blueprint_asset, newStaticComp, "bn_%s" % mesh_name
            )
            sean_utils.compile_blueprint(blueprint_asset)
            self.enableNanite(mesh)
            
        unreal.EditorLevelLibrary.destroy_actor(actor)
        unreal.log("done")
        return actor
