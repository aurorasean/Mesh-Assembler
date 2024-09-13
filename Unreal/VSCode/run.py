import unreal
import debugpy_unreal

actor = None


class Runner:
    def __init__(self, anims, meshes, textures, skels, asset_ref: unreal.Blueprint):
        # self.anims = anims
        self.meshes = meshes
        # self.textures = textures
        self.skels = skels
        self.asset_ref = asset_ref

    def enableNanite(self, staticMeshActor):
        """
        Enable Nanite on static mesh associated with static mesh actor.
        """
        staticMesh = staticMeshActor
        if staticMesh:
            # get mesh nanite settings
            meshNaniteSettings = staticMesh.get_editor_property("nanite_settings")
            if not meshNaniteSettings.enabled:
                meshNaniteSettings.enabled = True
                # classes that inherit from EditorSubsystem need to be instatiated
                unreal.StaticMeshEditorSubsystem().set_nanite_settings(
                    staticMesh, meshNaniteSettings, apply_changes=True
                )

    def run(self):
        debugpy_unreal.breakpoint()
        print("running blueprint creator: %s" % self.asset_ref.get_path_name())
        # need to update the asset_ref.skeletal mesh to new mesh
        blueprint_asset = unreal.load_asset(self.asset_ref.get_path_name())

        sean_utils = unreal.SeanUtils()
        component = unreal.StaticMeshComponent
        skelComponent = unreal.SkeletalMeshComponent
        
        newSkelCompo = sean_utils.add_component_to_actor(
            blueprint_asset, skelComponent, "SkeletalMesh", ""
        )
        newSkelCompo.set_skeletal_mesh(self.skels[0])
        newSkelCompo.set_visibility(False)
        # sean_utils.get_component_by_name_or_default(blueprint_asset, "")
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

        unreal.log("done")
        return actor
