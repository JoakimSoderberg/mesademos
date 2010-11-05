include_directories(
	${mesademos_SOURCE_DIR}/src/util
	${OPENGL_INCLUDE_PATH}
	${GLUT_INCLUDE_DIR}
	${GLEW_INCLUDE_DIR}
)

link_directories (
	${mesademos_SOURCE_DIR}/src/util
)

link_libraries (
	util
	${OPENGL_gl_LIBRARY}
	${OPENGL_glu_LIBRARY}
	${GLUT_glut_LIBRARY}
	${GLEW_glew_LIBRARY}
)

add_executable(arbfplight arbfplight.c)
add_executable(arbfslight arbfslight.c)
add_executable(arbocclude arbocclude.c)
add_executable(arbocclude2 arbocclude2.c)
add_executable(bounce bounce.c)
add_executable(clearspd clearspd.c)
add_executable(copypix copypix.c)
add_executable(cubemap cubemap.c)
add_executable(dinoshade dinoshade.c)
add_executable(dissolve dissolve.c)
add_executable(drawpix drawpix.c)
add_executable(engine engine.c)
add_executable(fbo_firecube fbo_firecube.c)
add_executable(fbotexture fbotexture.c)
add_executable(fire fire.c)
add_executable(fogcoord fogcoord.c)
add_executable(fplight fplight.c)
add_executable(fslight fslight.c)
add_executable(gamma gamma.c)
add_executable(gearbox gearbox.c)
add_executable(gears gears.c)
add_executable(geartrain geartrain.c)
add_executable(glinfo glinfo.c)
add_executable(gloss gloss.c)
add_executable(gltestperf gltestperf.c)
add_executable(ipers ipers.c)
add_executable(isosurf isosurf.c)
add_executable(lodbias lodbias.c)
add_executable(morph3d morph3d.c)
add_executable(multiarb multiarb.c)
add_executable(paltex paltex.c)
add_executable(pointblast pointblast.c)
add_executable(projtex projtex.c)
add_executable(ray ray.c)
add_executable(readpix readpix.c)
add_executable(reflect reflect.c)
add_executable(renormal renormal.c)
add_executable(shadowtex shadowtex.c)
add_executable(singlebuffer singlebuffer.c)
add_executable(spectex spectex.c)
add_executable(spriteblast spriteblast.c)
add_executable(stex3d stex3d.c)
add_executable(teapot teapot.c)
add_executable(terrain terrain.c)
add_executable(tessdemo tessdemo.c)
add_executable(texcyl texcyl.c)
add_executable(texenv texenv.c)
add_executable(textures textures.c)
add_executable(trispd trispd.c)
add_executable(tunnel2 tunnel2.c)
add_executable(tunnel tunnel.c)
add_executable(vao_demo vao_demo.c)
add_executable(winpos winpos.c)
